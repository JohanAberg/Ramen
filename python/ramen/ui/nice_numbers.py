
import math

class nice_numbers( object):
    def __init__( self, imin, imax, num_ticks):
        __range = self.__nice_num( imax - imin, False)
        self.__d = self.__nice_num( __range / ( num_ticks - 1), True)
        graphmax = math.ceil( imax / self.__d) * self.__d
        self.__x = math.floor( imin / self.__d) * self.__d
        self.__xmax = graphmax + 0.5 * self.__d

    def __nice_num( self, x, round):
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
        if self.__x > self.__xmax:
            raise StopIteration
        else:
            val = self.__x
            self.__x += self.__d
            return val
