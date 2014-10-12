# Copyright (c) 2012-2014 by the GalSim developers team on GitHub
# https://github.com/GalSim-developers
#
# This file is part of GalSim: The modular galaxy image simulation toolkit.
# https://github.com/GalSim-developers/GalSim
#
# GalSim is free software: redistribution and use in source and binary forms,
# with or without modification, are permitted provided that the following
# conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice, this
# list of conditions, and the disclaimer given in the accompanying LICENSE
# file.
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions, and the disclaimer given in the documentation
# and/or other materials provided with the distribution.
#

import galsim
import numpy

def applyNL(img,NLfunc,args=None):
    """ Applies the given non-linear function (NLfunc) on the input image and returns a new image of the same datatype 

    The argument NLfunc is a callable function, possibly with few arguments that need to be given as input to the 'args'.
    Callable functions from empirical curves and lookup tables can be constructed from galsim.LookupTable
    """

    #extract out the array from Image since not all functions can act directly on Images
    if args!=None:
        img_nl = NLfunc(img.array,args) 
    else:
    	img_nl = NLfunc(img.array)

    if img.array.shape != img_nl.shape:
        raise ValueError("Image shapes are inconsistent")

return galsim.Image(img_nl,dtype=img.dtype)

def addRecipFail(img,exp_time=200,alpha=0.0065):
     """ Takes into account of reciprocity failure.
     Calling
     -------

        >>> new_image = addRecipFail(img,exp_time,alpha)
    """

     #extracting the array out since log won't operate on Image
     arr_in = img.array
     arr_out = arr_in*(1.0+alpha*numpy.log10(1.0*arr_in/exp_time))
     return Image(arr_out,dtype=img.dtype)