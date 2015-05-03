//
//  squarescale.c
//  squarescale
//
//  Created by Kevin Nelson on 5/3/15.
//  Copyright (c) 2015 rknLA. All rights reserved.
//

#include "m_pd.h"

static t_class *squarescale_class;

typedef struct _squarescale {
    t_object x_obj;
    t_float f_last_input;
    t_int i_min, i_max;
    t_outlet *f_scale_out, *f_factor_out;
} t_squarescale;


void squarescale_bang(t_squarescale *x)
{
    if (x->f_last_input == -1) {
        // the uninitialized case, do nothing
        post("squarescale: bang ignored because no value has been sent yet");
        return;
    }

    t_float out_val = x->f_last_input;
    t_float factor = 1;
    if (out_val >= x->i_min) {
        while (out_val > x->i_max) {
            out_val = out_val / 2;
            factor = factor / 2;
        }
    } else {
        while (out_val < x->i_min) {
            out_val = out_val * 2;
            factor = factor * 2;
        }
    }

    outlet_float(x->f_factor_out, factor);
    outlet_float(x->f_scale_out, out_val);
}

void squarescale_set(t_squarescale *x, t_floatarg input)
{
    if (input <= 0) {
        post("squarescale input must be greater than zero!");
        return;
    }
    x->f_last_input = input;
}


void squarescale_float(t_squarescale *x, t_floatarg input)
{
    squarescale_set(x, input);
    squarescale_bang(x);
}

void squarescale_range(t_squarescale *x, t_floatarg min, t_floatarg max)
{
    if (min >= max) {
        post("Invalid range: min must be less than max");
        return;
    } else if (min < 0 || max < 0) {
        post("Invalid range: min and max values must be >= 0");
        return;
    }

    x->i_min = min;
    x->i_max = max;
}


void *squarescale_new(t_symbol *s, int argc, t_atom *argv)
{
    t_squarescale *x = (t_squarescale *)pd_new(squarescale_class);
    t_float min = 0, max = 100;

    switch (argc) {
        case 1:
            max = atom_getfloat(argv);
            break;
        case 2:
            min = atom_getfloat(argv);
            max = atom_getfloat(argv+1);
            break;
        default:
            break;
    }

    squarescale_range(x, min, max);
    x->f_last_input = -1;

    x->f_scale_out = outlet_new(&x->x_obj, &s_float);
    x->f_factor_out = outlet_new(&x->x_obj, &s_float);

    return (void *)x;
}

void squarescale_setup(void)
{
    squarescale_class = class_new(gensym("squarescale"),
                                  (t_newmethod)squarescale_new,
                                  0, sizeof(t_squarescale),
                                  CLASS_DEFAULT,
                                  A_GIMME, 0);

    class_addmethod(squarescale_class,
                    (t_method)squarescale_range,
                    gensym("range"),
                    A_DEFFLOAT, A_DEFFLOAT, 0);

    class_addmethod(squarescale_class,
                    (t_method)squarescale_set,
                    gensym("set"),
                    A_DEFFLOAT);

    class_addfloat(squarescale_class, (t_method)squarescale_float);
    class_addbang(squarescale_class, (t_method)squarescale_bang);
}
