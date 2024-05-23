/*
 * ColorUtils.h
 * Created from https://stackoverflow.com/questions/5816719/difference-between-function-arguments-declared-with-and-in-c
 *  Created on: 24 Oct 2021
 *      Author: jondurrant
 */

#ifndef COLORUTILS_H_
#define COLORUTILS_H_

class ColorUtils {
public:
	static void valToRGB(double val0To1, unsigned char& r, unsigned char& g, unsigned char& b);

	/**
	* Convert a wavelength in the visible light spectrum to a RGB color value that is suitable to be displayed on a
	* monitor
	*
	* @param wavelength wavelength in nm
	* @return RGB color encoded in int. each color is represented with 8 bits and has a layout of
	* 00000000RRRRRRRRGGGGGGGGBBBBBBBB where MSB is at the leftmost
	*/
	static void wavelengthToRGB(double wavelength, unsigned char& r, unsigned char& g, unsigned char& b);

	/**
	* Convert XYZ to RGB in the sRGB color space
	* <p>
	* The conversion matrix and color component transfer function is taken from http://www.color.org/srgb.pdf, which
	* follows the International Electrotechnical Commission standard IEC 61966-2-1 "Multimedia systems and equipment -
	* Colour measurement and management - Part 2-1: Colour management - Default RGB colour space - sRGB"
	*
	* @param xyz XYZ values in a double array in the order of X, Y, Z. each value in the range of [0.0, 1.0]
	* @return RGB values in a double array, in the order of R, G, B. each value in the range of [0.0, 1.0]
	*/
	static void srgbXYZ2RGB(double x, double y, double z, double& r, double& g, double& b);

	/**
	* helper function for {@link #srgbXYZ2RGB(double[])}
	*/
	static double srgbXYZ2RGBPostprocess(double c);

	/**
	* A multi-lobe, piecewise Gaussian fit of CIE 1931 XYZ Color Matching Functions by Wyman el al. from Nvidia. The
	* code here is adopted from the Listing 1 of the paper authored by Wyman et al.
	* <p>
	* Reference: Chris Wyman, Peter-Pike Sloan, and Peter Shirley, Simple Analytic Approximations to the CIE XYZ Color
	* Matching Functions, Journal of Computer Graphics Techniques (JCGT), vol. 2, no. 2, 1-11, 2013.
	*
	* @param wavelength wavelength in nm
	* @return XYZ in a double array in the order of X, Y, Z. each value in the range of [0.0, 1.0]
	*/
	static void cie1931WavelengthToXYZFit(double wavelength, double& x, double& y, double& z);
};

#endif /* COLORUTILS_H_ */
