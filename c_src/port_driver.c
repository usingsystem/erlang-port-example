#include <stdio.h>
#include "erl_driver.h"
#include "complex.h"

typedef struct {
	ErlDrvPort port;
} example_data;

static ErlDrvData complex_drv_start(ErlDrvPort port, char *buff) {
	example_data *d = (example_data *)driver_alloc(sizeof(example_data));
	d->port = port;
	return (ErlDrvData)d;
}

static void complex_drv_stop(ErlDrvData handle) {
	driver_free((char *)handle);
}

static void complex_drv_output(ErlDrvData handle, char *buffer, int bufflen) {
	example_data *d = (example_data *)handle;
	char fn = buffer[0], arg = buffer[1], res;
	if(fn == 1) {
		res = foo(arg);
	} else if(fn == 2) {
		res = bar(arg);
	}
	driver_output(d->port, &res, 1);
}

ErlDrvEntry complex_driver_entry = {
	NULL,
	complex_drv_start,
	complex_drv_stop,
	complex_drv_output,
	NULL,
	NULL,
	"complex_drv",
	NULL,
	NULL,
	NULL,
	NULL
};

DRIVER_INIT(complex_drv)
{
	return &complex_driver_entry;
}

