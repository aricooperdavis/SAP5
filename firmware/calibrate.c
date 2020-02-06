#include "config.h"
#include <xc.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_statistics.h>
#include "display.h"
#include "font.h"
#include "sensors.h"
#include "maths.h"
#include "leg.h"
#include "calibrate.h"
#include "utils.h"
#include "laser.h"
#include "gsl_static.h"
#include "beep.h"
#include "input.h"
#include "menu.h"

GSL_MATRIX_DECLARE(mag_readings, CALIBRATION_SAMPLES, 3);
GSL_MATRIX_DECLARE(grav_readings, CALIBRATION_SAMPLES, 3);
double mag_cal_store[0x400/sizeof(double)] PLACE_DATA_AT(APP_CALIBRATION_LOCATION) = {0};
double grav_cal_store[0x400/sizeof(double)] PLACE_DATA_AT(APP_CALIBRATION_LOCATION+0x400) = {0};


DECLARE_MENU(calibration_menu, {
    /* calibrate menu */
    {"Sensors", Action, {calibrate_sensors}, 0},
    {"Laser", Action, {calibrate_laser}, 0},
    {"Axes", Action, {calibrate_axes}, 0},
    {"Back", Back, {NULL}, 0},
});


static
int get_greatest_axis(struct RAW_SENSORS *raw) {
    int max_abs = 0;
    int axis = 0;
    int i;
    for (i=0; i<3; i++) {
        if (abs(raw->accel[i])> max_abs) {
            if (raw->accel[i] < 0) {
                axis = i;
            } else {
                axis = i+3;
            }
            max_abs = abs(raw->accel[i]);
        }
    }   
    return axis;
}

const int compass_axis_map[] = {4,3,2,1,0,5};

static
int get_compass_axis(int axis) {
    return compass_axis_map[axis];
}

static
void set_axis(int i) {
    struct RAW_SENSORS raw;
    sensors_read_raw(&raw);
    config.axes.accel[i] = get_greatest_axis(&raw);
    config.axes.mag[i] = get_compass_axis(config.axes.accel[i]);
}

static
bool check_sane_axes(void) {
    int i,j;
    bool accel_error, mag_error;
    for (i=0; i<3; i++) {
        accel_error = mag_error = true;
        for (j=0; j<3; j++) {
            if ((config.axes.accel[j] %3) == i) accel_error = false;
            if ((config.axes.mag[j] %3) == i) mag_error = false;
        }
        if (accel_error || mag_error) return false;
    }
    return true;
}

void calibrate_axes(int32_t dummy) {
    int i;
    char text[18];
    const char *instructions[] = {
        "Please place on\nedge with the\nlaser pointing\nleft",
        "Please point the\nlaser upwards",
        "Please place the\ndisplay flat on\na level surface"
    };
    for (i=2; i>=0; i--) {
        display_clear_screen(true);
        display_write_multiline(0, instructions[i], true);
        delay_ms_safe(3000);
        set_axis(i);
    }
    if (!check_sane_axes()) {
        display_clear_screen(true);
        display_write_multiline(0, "Invalid axes\nfound.\nAborting", true);
        return;
    }
    snprintf(text,18, "%d %d %d", config.axes.accel[0], config.axes.accel[1], config.axes.accel[2]);
    display_write_text(1,0,text, &small_font, false, true);
    snprintf(text,18, "%d %d %d", config.axes.mag[0], config.axes.mag[1], config.axes.mag[2]);
    display_write_text(4,0,text, &small_font, false, true);
    delay_ms_safe(6000);
    config_save();
    
}


double check_calibration(const gsl_matrix *data, int len, calibration *cal) {
    GSL_VECTOR_DECLARE(result,3);
    int k;
    double magnitude;
    double error=0;
    for (k=0; k<len; k++) {
        gsl_vector_const_view row = gsl_matrix_const_row(data, k);
        apply_calibration(&row.vector, cal, &result);
        magnitude = gsl_blas_dnrm2(&result);
        error += fabs(magnitude-1.0);
    }
    return (error/len)*100;
}

double check_accuracy(const gsl_matrix *mag, const calibration *mag_cal,
                      const gsl_matrix *grav, const calibration *grav_cal) {
    GSL_VECTOR_DECLARE(mag_row, 3);
    GSL_VECTOR_DECLARE(grav_row, 3);
    GSL_VECTOR_DECLARE(result, 3);
    GSL_MATRIX_DECLARE(orientation, 8, 3);
    int i;
    /* create matrix with vectors of orientation*/
    for (i=0; i<8; i++) {
        gsl_vector_const_view mag_temp = gsl_matrix_const_row(mag, i);
        gsl_vector_const_view grav_temp = gsl_matrix_const_row(grav, i);
        apply_calibration(&mag_temp.vector, mag_cal, &mag_row);
        apply_calibration(&grav_temp.vector, grav_cal, &grav_row);
        gsl_vector_view orient_row = gsl_matrix_row(&orientation, i);
        maths_get_orientation_as_vector(&mag_row,
                                        &grav_row,
                                        &orient_row.vector);
    }
    /* calculate absolute deviation for each axis*/
    for (i=0; i<3; i++) {
        gsl_vector_view column = gsl_matrix_column(&orientation, i);
        gsl_vector_set(&result, i, gsl_stats_absdev(column.vector.data, 
                                                    column.vector.stride,
                                                    column.vector.size));
    }
    
    return gsl_blas_dnrm2(&result) * 180.0 / M_PI;
}

void collect_data(gsl_matrix *mag_data, gsl_matrix *grav_data, int offset, int count) {
    struct COOKED_SENSORS sensors;
    gsl_vector_view mag_sensors = gsl_vector_view_array(sensors.mag,3);
    gsl_vector_view grav_sensors = gsl_vector_view_array(sensors.accel,3);
    int i;
    display_off();
    laser_on();
    beep_beep();
    for (i=0; i< count; i++) {
        //delay to let user move to position
        delay_ms_safe(5000);
        //read in samples
        sensors_read_uncalibrated(&sensors, SAMPLES_PER_READING);
        gsl_matrix_set_row(mag_data, i+offset, &mag_sensors.vector);
        gsl_matrix_set_row(grav_data, i+offset, &grav_sensors.vector);
        //beep to let user know to move to next position.
        laser_off();
        delay_ms_safe(300);
        beep_beep();
        laser_on();
        timeout_reset();
    }
    beep_happy();
    laser_off();
    display_on();
    display_clear_screen(true);
}

int get_calibration_data(gsl_matrix *mag, gsl_matrix *grav) {
    /* get readings around  z-axis*/
    display_clear_screen(true);
    display_write_multiline(0, "Place device on\n"
                               "inclined surface\n"
                               "After each beep\n"
                               "rotate by ~45'", true);
    delay_ms_safe(2000);
    collect_data(mag, grav, 0, CAL_AXIS_COUNT);
    
    /* now read data on x-axis*/
    display_write_multiline(0, "Place device flat\n"
                               "After each beep\n"
                               "rotate end over\n"
                               "end by ~45'", true);
    delay_ms_safe(2000);
    collect_data(mag, grav, CAL_AXIS_COUNT, CAL_AXIS_COUNT);
    
    /* now read data on y-axis */
    display_write_multiline(0, "Point laser at\nfixed target", true);
    delay_ms_safe(2000);
    display_clear_screen(true);
    display_write_multiline(0, "After each beep\n"
                               "rotate by ~45'\n"
                               "leaving laser\n"
                               "on target", true);
    delay_ms_safe(1500);
    collect_data(mag, grav, CAL_AXIS_COUNT*2, CAL_TARGET_COUNT);
    return 16;
}

void calibrate_sensors(int32_t dummy) {
    char text[30];
    CALIBRATION_DECLARE(grav_cal);
    CALIBRATION_DECLARE(mag_cal);
    gsl_matrix_const_view mag_spins = gsl_matrix_const_submatrix(&mag_readings, 
            CAL_AXIS_COUNT*2, 0, 
            CAL_TARGET_COUNT, 3);
    gsl_matrix_const_view grav_spins = gsl_matrix_const_submatrix(&grav_readings, 
            CAL_AXIS_COUNT*2, 0, 
            CAL_TARGET_COUNT, 3);
    int data_length;
    double grav_error, mag_error, accuracy;
    /* get data */
    data_length = get_calibration_data(&mag_readings, &grav_readings);
    display_write_multiline(0, "Processing", true);
    delay_ms_safe(2000);
    
    //do calibration    
    fit_ellipsoid(&mag_readings, data_length, &mag_cal);
    fit_ellipsoid(&grav_readings, data_length, &grav_cal);
    align_laser(&mag_spins.matrix, &mag_cal);
    align_laser(&grav_spins.matrix, &grav_cal);
    sync_sensors(&mag_readings, &mag_cal, &grav_readings, &grav_cal);
    
    // show mag error
    mag_error = check_calibration(&mag_readings, data_length, &mag_cal);
    sprintf(text, "Mag Err:  %.2f%%", mag_error);
    display_write_multiline(2,text, true);
    
    //show grav error
    grav_error = check_calibration(&grav_readings, data_length, &grav_cal);
    sprintf(text, "Grav Err: %.2f%%", grav_error);
    display_write_multiline(4,text, true);
    
    //show accuracy
    accuracy = check_accuracy(&mag_spins.matrix, &mag_cal,
                              &grav_spins.matrix, &grav_cal);
    sprintf(text, "Accuracy: %.2f`", accuracy);
    display_write_multiline(4,text, true);
    delay_ms_safe(4000);
    
    //check satisfactory calibration
    display_clear_screen(true);
    if (isnan(grav_error) || isnan(mag_error) || isnan(accuracy)) {
        display_write_multiline(2, "Calibration failed\nNot Saved", true);
    } else if ((grav_error > 5.0) || (mag_error > 5.0) || (accuracy > 1.2)) {
        display_write_multiline(2, "Poor calibration\nNot saved", true);
    } else {
        display_write_multiline(2, "Calibration Good\nSaved.", true);
        calibration conf_grav = calibration_from_doubles(config.calib.accel);
        calibration conf_mag = calibration_from_doubles(config.calib.mag);
        calibration_memcpy(&conf_grav, &grav_cal);
        calibration_memcpy(&conf_mag, &mag_cal);
        wdt_clear();
        config_save();
    }
    //save calibration data
    memory_erase_page(mag_cal_store);
    memory_write_data(mag_cal_store, mag_readings_data, sizeof(mag_readings_data));
    memory_write_data(grav_cal_store, 
                      grav_readings_data, sizeof(grav_readings_data));
    delay_ms_safe(4000);    
}

void calibrate_laser(int32_t dummy) {
    GSL_VECTOR_DECLARE(samples, 10);
    int i;
    double distance, error, offset;
    char text[80];
    display_write_multiline(0,"Place a target 1m\n"
                              "from the rearmost\n"
                              "point of the\n"
                              "device", true);
    laser_on();
    delay_ms_safe(4000);
    for (i=0; i<samples.size; ++i) {
        laser_on();
        delay_ms_safe(100);
        distance = laser_read_raw(LASER_MEDIUM, 4000);
        gsl_vector_set(&samples, i, distance);
        beep_beep();
    }
    laser_off();
    distance = gsl_stats_mean(samples.data, samples.stride, samples.size);
    error = gsl_stats_sd(samples.data, samples.stride, samples.size);
    offset = 1.000 - distance;
    config.calib.laser_offset = offset;
    sprintf(text, "Offset: %.3f\n"
                  "Error: %.3f\n"
                  "Config saved", offset, error);
    display_clear_screen(true);
    display_write_multiline(0, text, true);
    delay_ms_safe(2000);
    config_save();
    display_write_multiline(6, "Saved", true);
    delay_ms_safe(1000);
}

