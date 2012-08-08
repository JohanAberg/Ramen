
import math

class nice_numbers( Object):
    def __init__( self, imin, imax, num_ticks):
        _range = self._nice_num( imax - imin, False)
        self._d = self._nice_num( self._range / ( num_ticks - 1), True)
        graphmax = math.ceil( imax / self._d) * self._d
        self._x = math.floor( imin / self._d) * self._d
        self._xmax = graphmax + 0.5 * self._d

    def _nice_num( x, round):
        expv = math.floor( math.log10( x))
        f = x / math.pow( 10.0, expv); # between 1 and 10
        nf = 0.0

        if round:
            if f < 1.5:
                nf = 1.0
            else:
                if f < 3.0:
                    nf = 2.0
                else:
                    if f < 7.0:
                        nf = 5.0
                    else:
                        nf = 10.0
        else:
            if f <= 1.0:
                nf = 1.0
            else:
                if f <= 2.0:
                    nf = 2.0
                else:
                    if f <= 5.0:
                        nf = 5.0
                    else:
                        nf = 10.0

        return nf * math.pow( 10.0, expv);

    def __iter__( self):
        return self

    def next( self):
        if self._x > self._xmax:
            raise StopIteration
        else:
            val = self._x
            self._x += self._d
            return val
