#include	<math.h>
#include	"gr.hh"
#include	"extern.hh"
#include	"private.hh"

// Return NO if cannot make a scale
bool
create_x_scale()
{
	int             nsegs;
	double min, max;
	if (_need_x_axis && !_user_set_x_axis) {
		if (!gr_missing(_colX.min())) {
			min = _colX.min();
			max = _colX.max();
		} else {
			if (_xgrid_exists) {
				min = max = _xmatrix[0];
				for (unsigned int i = 1; i < _num_xmatrix_data; i++) {
					if (_xmatrix[i] < min)
						min = _xmatrix[i];
					if (_xmatrix[i] > max)
						max = _xmatrix[i];
				}
			} else {
				return false;
			}
		}
		if (_xtype == gr_axis_LINEAR) {
			int             nsegs_est;
			double          xsize;
			if (!get_var("..xsize..", &xsize))
				xsize = 10.0;	/* guess */
			nsegs_est = 1 + (int) (xsize / 2.0); // ??? Should use fontsize
			if (_xincreasing)
				gr_scale125(min, max, nsegs_est, &_xleft, &_xright, &nsegs);
			else
				gr_scale125(min, max, nsegs_est, &_xright, &_xleft, &nsegs);
			PUT_VAR("..xleft..", _xleft);
			PUT_VAR("..xright..", _xright);
			_xinc = (_xright - _xleft) / nsegs;
		} else {
			// LOG axis
			if (min <= 0.0 || max <= 0.0) {
				// log data not OK
				int             nsegs_est;
				double          xsize;
				warning("Have non-positive data -- switching to linear axis");
				if (!get_var("..xsize..", &xsize))
					xsize = 10.0;	/* guess */
				nsegs_est = 1 + (int) (xsize / 2.0);
				_xtype = gr_axis_LINEAR;
				gr_scale125(min, max, nsegs_est, &_xleft, &_xright, &nsegs);
				PUT_VAR("..xleft..", _xleft);
				PUT_VAR("..xright..", _xright);
				_xinc = ((double) _xright - (double) _xleft) / nsegs;
			} else {
				PUT_VAR("..xleft..", pow(10.0, floor(log10((double) min))));
				PUT_VAR("..xright..", pow(10.0, ceil(log10((double) max))));
				_xinc = 1.0;
			}
		}
		_xscale_exists = true;
	}
	return true;
}

// Return NO if cannot make a scale
bool
create_y_scale()
{
	int             nsegs;
	double min, max;
	if (_need_y_axis && !_user_set_y_axis) {
		if (!gr_missing(_colY.min())) {
			min = _colY.min();
			max = _colY.max();
		} else {
			if (_ygrid_exists) {
				min = max = _ymatrix[0];
				for (unsigned int i = 1; i < _num_ymatrix_data; i++) {
					if (_ymatrix[i] < min)
						min = _ymatrix[i];
					if (_ymatrix[i] > max)
						max = _ymatrix[i];
				}
			} else {
				return false;
			}
		}
		if (_ytype == gr_axis_LINEAR) {
			int             nsegs_est;
			double          ysize;
			if (!get_var("..ysize..", &ysize))
				ysize = 10.0;	// guess
			nsegs_est = 1 + (int) (ysize / 2.0);
			if (_yincreasing)
				gr_scale125(min, max, nsegs_est, &_ybottom, &_ytop, &nsegs);
			else
				gr_scale125(min, max, nsegs_est, &_ytop, &_ybottom, &nsegs);
			PUT_VAR("..ybottom..", _ybottom);
			PUT_VAR("..ytop..", _ytop);
			_yinc = (_ytop - _ybottom) / nsegs;
		} else {
			// LOG axis
			if (min <= 0.0 || max <= 0.0) {
				// log data not OK
				int             nsegs_est;
				double          ysize;
				warning("Have non-positive data -- switching to linear axis");
				if (!get_var("..ysize..", &ysize))
					ysize = 10.0;	// guess
				nsegs_est = 1 + (int) (ysize / 2.0);
				_ytype = gr_axis_LINEAR;
				gr_scale125(min, max, nsegs_est, &_ybottom, &_ytop, &nsegs);
				PUT_VAR("..ybottom..", _ybottom);
				PUT_VAR("..ytop..", _ytop);
				_yinc = (_ytop - _ybottom) / nsegs;
			} else {
				PUT_VAR("..ybottom..", pow(10.0, floor(log10(min))));
				PUT_VAR("..ytop..", pow(10.0, ceil(log10(max))));
				_yinc = 1.0;
			}
		}
		_yscale_exists = true;
	}
	return true;
}
