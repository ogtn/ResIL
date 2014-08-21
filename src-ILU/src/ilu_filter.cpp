
// Description: Applies filters to an image.

#include "ilu_internal.h"
#include "ilu_filter.h"
#include <math.h>
#include <limits.h>
#include "IL/il2.h"

// Could probably be optimized a bit...too many nested for loops.
//! Pixelizes an image
ILboolean ILAPIENTRY ilu2Pixelize(ILimage* image, ILuint PixSize)
{
	ILuint		x, y, z, i, j, k, c, r, Total, Tested;
	ILushort	*ShortPtr;
	ILuint		*IntPtr;
	ILdouble	*DblPtr, DblTotal, DblTested;
	ILubyte		*RegionMask;

	if (image == NULL) {
		il2SetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (PixSize == 0)
		PixSize = 1;
	r = 0;

	if (image->Format == IL_COLOUR_INDEX)
		il2ConvertImage(image, ilGetPalBaseType(image->Pal.PalType), IL_UNSIGNED_BYTE);

	RegionMask = iScanFill(image);

	switch (image->Bpc)
	{
		case 1:
			for (z = 0; z < image->Depth; z += PixSize) {
				for (y = 0; y < image->Height; y += PixSize) {
					for (x = 0; x < image->Width; x += PixSize) {
						for (c = 0; c < image->Bpp; c++) {
							Total = 0;  Tested = 0;
							for (k = 0; k < PixSize && z+k < image->Depth; k++) {
								for (j = 0; j < PixSize && y+j < image->Height; j++) {
									for (i = 0; i < PixSize && x+i < image->Width; i++, Tested++) {
										Total += image->Data[(z+k) * image->SizeOfPlane + 
											(y+j) * image->Bps + (x+i) * image->Bpp + c];
									}
								}
							}
							Total /= Tested;

							for (k = 0; k < PixSize && z+k < image->Depth; k++) {
								for (j = 0; j < PixSize && y+j < image->Height; j++) {
									for (i = 0; i < PixSize && x+i < image->Width; i++, Tested++) {
										if (RegionMask) {
											if (!RegionMask[(y+j) * image->Width + (x+i)])
												continue;
										}
										image->Data[(z+k) * image->SizeOfPlane + (y+j)
											* image->Bps + (x+i) * image->Bpp + c] =
											Total;
									}
								}
							}
						}
					}
				}
			}
			break;
		case 2:
			ShortPtr = (ILushort*)image->Data;
			image->Bps /= 2;
			for (z = 0; z < image->Depth; z += PixSize) {
				for (y = 0; y < image->Height; y += PixSize) {
					for (x = 0; x < image->Width; x += PixSize, r += PixSize) {
						for (c = 0; c < image->Bpp; c++) {
							Total = 0;  Tested = 0;
							for (k = 0; k < PixSize && z+k < image->Depth; k++) {
								for (j = 0; j < PixSize && y+j < image->Height; j++) {
									for (i = 0; i < PixSize && x+i < image->Width; i++, Tested++) {
										Total += ShortPtr[(z+k) * image->SizeOfPlane + 
											(y+j) * image->Bps + (x+i) * image->Bpp + c];
									}
								}
							}
							Total /= Tested;

							for (k = 0; k < PixSize && z+k < image->Depth; k++) {
								for (j = 0; j < PixSize && y+j < image->Height; j++) {
									for (i = 0; i < PixSize && x+i < image->Width; i++, Tested++) {
										if (RegionMask[r+i]) {
											ShortPtr[(z+k) * image->SizeOfPlane + (y+j)
												* image->Bps + (x+i) * image->Bpp + c] =
												Total;
										}
									}
								}
							}
						}
					}
				}
			}
			image->Bps *= 2;
			break;
		case 4:
			IntPtr = (ILuint*)image->Data;
			image->Bps /= 4;
			for (z = 0; z < image->Depth; z += PixSize) {
				for (y = 0; y < image->Height; y += PixSize) {
					for (x = 0; x < image->Width; x += PixSize, r += PixSize) {
						for (c = 0; c < image->Bpp; c++) {
							Total = 0;  Tested = 0;
							for (k = 0; k < PixSize && z+k < image->Depth; k++) {
								for (j = 0; j < PixSize && y+j < image->Height; j++) {
									for (i = 0; i < PixSize && x+i < image->Width; i++, Tested++) {
										Total += IntPtr[(z+k) * image->SizeOfPlane + 
											(y+j) * image->Bps + (x+i) * image->Bpp + c];
									}
								}
							}
							Total /= Tested;

							for (k = 0; k < PixSize && z+k < image->Depth; k++) {
								for (j = 0; j < PixSize && y+j < image->Height; j++) {
									for (i = 0; i < PixSize && x+i < image->Width; i++, Tested++) {
										if (RegionMask[r+i]) {
											IntPtr[(z+k) * image->SizeOfPlane + (y+j)
												* image->Bps + (x+i) * image->Bpp + c] =
												Total;
										}
									}
								}
							}
						}
					}
				}
			}
			image->Bps *= 4;
			break;
		case 8:
			DblPtr = (ILdouble*)image->Data;
			image->Bps /= 8;
			for (z = 0; z < image->Depth; z += PixSize) {
				for (y = 0; y < image->Height; y += PixSize) {
					for (x = 0; x < image->Width; x += PixSize, r += PixSize) {
						for (c = 0; c < image->Bpp; c++) {
							DblTotal = 0.0;  DblTested = 0.0;
							for (k = 0; k < PixSize && z+k < image->Depth; k++) {
								for (j = 0; j < PixSize && y+j < image->Height; j++) {
									for (i = 0; i < PixSize && x+i < image->Width; i++, DblTested++) {
										DblTotal += DblPtr[(z+k) * image->SizeOfPlane + 
											(y+j) * image->Bps + (x+i) * image->Bpp + c];
									}
								}
							}
							DblTotal /= DblTested;

							for (k = 0; k < PixSize && z+k < image->Depth; k++) {
								for (j = 0; j < PixSize && y+j < image->Height; j++) {
									for (i = 0; i < PixSize && x+i < image->Width; i++, DblTested++) {
										if (RegionMask[r+i]) {
											DblPtr[(z+k) * image->SizeOfPlane + (y+j)
												* image->Bps + (x+i) * image->Bpp + c] =
												DblTotal;
										}
									}
								}
							}
						}
					}
				}
			}
			image->Bps *= 8;
			break;
	}

	ifree(RegionMask);

	return IL_TRUE;
}


// Everything here was taken from a tutorial on "Elementary Digital Filtering"
//	by Ender Wiggen, found at http://www.gamedev.net/reference/programming/features/edf/

// Needs some SERIOUS optimization.
ILubyte *Filter(ILimage *Image, const ILint *matrix, ILint scale, ILint bias)
{
    ILint		x, y, c, LastX, LastY, Offsets[9];
	ILuint		i, Temp, z;
	ILubyte		*Data, *ImgData, *NewData, *RegionMask;
	ILdouble	Num;
	
	if (Image == NULL) {
		il2SetError(ILU_ILLEGAL_OPERATION);
		return NULL;
	}

	Data = (ILubyte*)ialloc(Image->SizeOfData);
	if (Data == NULL) {
		return NULL;
	}

	RegionMask = iScanFill(Image);

	// Preserve original data.
	ImgData = Image->Data;
	NewData = Data;

	for (z = 0; z < Image->Depth; z++) {
		LastX = Image->Width  - 1;
		LastY = Image->Height - 1;
		for (y = 1; y < LastY; y++) {
			for (x = 1; x < LastX; x++) {
				Offsets[4] = ((y  ) * Image->Width + (x  )) * Image->Bpp;
				if (RegionMask) {
					if (!RegionMask[y * Image->Width + x]) {
						for (c = 0; c < Image->Bpp; c++) {
							Data[Offsets[4]+c] = Image->Data[Offsets[4]+c];
						}
						continue;
					}
				}

				Offsets[0] = ((y-1) * Image->Width + (x-1)) * Image->Bpp;
				Offsets[1] = ((y-1) * Image->Width + (x  )) * Image->Bpp;
				Offsets[2] = ((y-1) * Image->Width + (x+1)) * Image->Bpp;
				Offsets[3] = ((y  ) * Image->Width + (x-1)) * Image->Bpp;
				Offsets[5] = ((y  ) * Image->Width + (x+1)) * Image->Bpp;
				Offsets[6] = ((y+1) * Image->Width + (x-1)) * Image->Bpp;
				Offsets[7] = ((y+1) * Image->Width + (x  )) * Image->Bpp;
				Offsets[8] = ((y+1) * Image->Width + (x-1)) * Image->Bpp;

				// Always has at least one, so get rid of all those +c's
				Num =   Image->Data[Offsets[0]] * matrix[0] +
						Image->Data[Offsets[1]] * matrix[1]+
						Image->Data[Offsets[2]] * matrix[2]+
						Image->Data[Offsets[3]] * matrix[3]+
						Image->Data[Offsets[4]] * matrix[4]+
						Image->Data[Offsets[5]] * matrix[5]+
						Image->Data[Offsets[6]] * matrix[6]+
						Image->Data[Offsets[7]] * matrix[7]+
						Image->Data[Offsets[8]] * matrix[8];

				Temp = (ILuint)fabs((Num / (ILdouble)scale) + bias);
				if (Temp > 255)
					Data[Offsets[4]] = 255;
				else
					Data[Offsets[4]] = Temp;

				for (c = 1; c < Image->Bpp; c++) {
					Num =   Image->Data[Offsets[0]+c] * matrix[0]+
							Image->Data[Offsets[1]+c] * matrix[1]+
							Image->Data[Offsets[2]+c] * matrix[2]+
							Image->Data[Offsets[3]+c] * matrix[3]+
							Image->Data[Offsets[4]+c] * matrix[4]+
							Image->Data[Offsets[5]+c] * matrix[5]+
							Image->Data[Offsets[6]+c] * matrix[6]+
							Image->Data[Offsets[7]+c] * matrix[7]+
							Image->Data[Offsets[8]+c] * matrix[8];

					Temp = (ILuint)fabs((Num / (ILdouble)scale) + bias);
					if (Temp > 255)
						Data[Offsets[4]+c] = 255;
					else
						Data[Offsets[4]+c] = Temp;
				}
			}
		}


		// Copy 4 corners
		for (c = 0; c < Image->Bpp; c++) {
			Data[c] = Image->Data[c];
			Data[Image->Bps - Image->Bpp + c] = Image->Data[Image->Bps - Image->Bpp + c];
			Data[(Image->Height - 1) * Image->Bps + c] = Image->Data[(Image->Height - 1) * Image->Bps + c];
			Data[Image->Height * Image->Bps - Image->Bpp + c] = 
				Image->Data[Image->Height * Image->Bps - Image->Bpp + c];
		}


		// If we only copy the edge pixels, then they receive no filtering, making them
		//	look out of place after several passes of an image.  So we filter the edge
		//	rows/columns, duplicating the edge pixels for one side of the "matrix".

		// First row
		for (x = 1; x < (ILint)Image->Width-1; x++) {
			if (RegionMask) {
				if (!RegionMask[x]) {
					Data[y + x * Image->Bpp + c] = Image->Data[y + x * Image->Bpp + c];
					continue;
				}
			}
			for (c = 0; c < Image->Bpp; c++) {
				Num =   Image->Data[(x-1) * Image->Bpp + c] * matrix[0] +
						Image->Data[x * Image->Bpp + c] * matrix[1]+
						Image->Data[(x+1) * Image->Bpp + c] * matrix[2]+
						Image->Data[(x-1) * Image->Bpp + c] * matrix[3]+
						Image->Data[x * Image->Bpp + c] * matrix[4]+
						Image->Data[(x+1) * Image->Bpp + c] * matrix[5]+
						Image->Data[(Image->Width + (x-1)) * Image->Bpp + c] * matrix[6]+
						Image->Data[(Image->Width + (x  )) * Image->Bpp + c] * matrix[7]+
						Image->Data[(Image->Width + (x-1)) * Image->Bpp + c] * matrix[8];

					Temp = (ILuint)fabs((Num / (ILdouble)scale) + bias);
					if (Temp > 255)
						Data[x * Image->Bpp + c] = 255;
					else
						Data[x * Image->Bpp + c] = Temp;
			}
		}

		// Last row
		y = (Image->Height - 1) * Image->Bps;
		for (x = 1; x < (ILint)Image->Width-1; x++) {
			if (RegionMask) {
				if (!RegionMask[(Image->Height - 1) * Image->Width + x]) {
					Data[y + x * Image->Bpp + c] = Image->Data[y + x * Image->Bpp + c];
					continue;
				}
			}
			for (c = 0; c < Image->Bpp; c++) {
				Num =   Image->Data[y - Image->Bps + (x-1) * Image->Bpp + c] * matrix[0] +
						Image->Data[y - Image->Bps + x * Image->Bpp + c] * matrix[1]+
						Image->Data[y - Image->Bps + (x+1) * Image->Bpp + c] * matrix[2]+
						Image->Data[y + (x-1) * Image->Bpp + c] * matrix[3]+
						Image->Data[y + x * Image->Bpp + c] * matrix[4]+
						Image->Data[y + (x+1) * Image->Bpp + c] * matrix[5]+
						Image->Data[y + (x-1) * Image->Bpp + c] * matrix[6]+
						Image->Data[y + x * Image->Bpp + c] * matrix[7]+
						Image->Data[y + (x-1) * Image->Bpp + c] * matrix[8];

					Temp = (ILuint)fabs((Num / (ILdouble)scale) + bias);
					if (Temp > 255)
						Data[y + x * Image->Bpp + c] = 255;
					else
						Data[y + x * Image->Bpp + c] = Temp;
			}
		}

		// Left side
		for (i = 1, y = Image->Bps; i < Image->Height-1; i++, y += Image->Bps) {
			if (RegionMask) {
				if (!RegionMask[y / Image->Bpp]) {
					Data[y + c] = Image->Data[y + c];
					continue;
				}
			}
			for (c = 0; c < Image->Bpp; c++) {
				Num =   Image->Data[y - Image->Bps + c] * matrix[0] +
						Image->Data[y - Image->Bps + Image->Bpp + c] * matrix[1]+
						Image->Data[y - Image->Bps + 2 * Image->Bpp + c] * matrix[2]+
						Image->Data[y + c] * matrix[3]+
						Image->Data[y + Image->Bpp + c] * matrix[4]+
						Image->Data[y + 2 * Image->Bpp + c] * matrix[5]+
						Image->Data[y + Image->Bps + c] * matrix[6]+
						Image->Data[y + Image->Bps + Image->Bpp + c] * matrix[7]+
						Image->Data[y + Image->Bps + 2 * Image->Bpp + c] * matrix[8];

					Temp = (ILuint)fabs((Num / (ILdouble)scale) + bias);
					if (Temp > 255)
						Data[y + c] = 255;
					else
						Data[y + c] = Temp;
			}
		}

		// Right side
		for (i = 1, y = Image->Bps * 2 - Image->Bpp; i < Image->Height-1; i++, y += Image->Bps) {
			if (RegionMask) {
				if (!RegionMask[y / Image->Bpp + (Image->Width - 1)]) {
					for (c = 0; c < Image->Bpp; c++) {
						Data[y + c] = Image->Data[y + c];
					}
					continue;
				}
			}
			for (c = 0; c < Image->Bpp; c++) {
				Num =   Image->Data[y - Image->Bps + c] * matrix[0] +
						Image->Data[y - Image->Bps - Image->Bpp + c] * matrix[1]+
						Image->Data[y - Image->Bps - 2 * Image->Bpp + c] * matrix[2]+
						Image->Data[y + c] * matrix[3]+
						Image->Data[y - Image->Bpp + c] * matrix[4]+
						Image->Data[y - 2 * Image->Bpp + c] * matrix[5]+
						Image->Data[y + Image->Bps + c] * matrix[6]+
						Image->Data[y + Image->Bps - Image->Bpp + c] * matrix[7]+
						Image->Data[y + Image->Bps - 2 * Image->Bpp + c] * matrix[8];

					Temp = (ILuint)fabs((Num / (ILdouble)scale) + bias);
					if (Temp > 255)
						Data[y + c] = 255;
					else
						Data[y + c] = Temp;
			}
		}

		// Go to next "plane".
		Image->Data += Image->SizeOfPlane;
		Data += Image->SizeOfPlane;
	}

	ifree(RegionMask);

	// Restore original data.
	Image->Data = ImgData;
	Data = NewData;

	return Data;
}


ILboolean ILAPIENTRY ilu2EdgeDetectP(ILimage* image)
{
	ILubyte		*HPass, *VPass;
	ILuint		i;
	ILboolean	Palette = IL_FALSE, Converted = IL_FALSE;
	ILenum		Type = 0;

	if (image == NULL) {
		il2SetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (image->Format == IL_COLOUR_INDEX) {
		Palette = IL_TRUE;
		il2ConvertImage(image, ilGetPalBaseType(image->Pal.PalType), IL_UNSIGNED_BYTE);
	}
	else if (image->Type > IL_UNSIGNED_BYTE) {
		Converted = IL_TRUE;
		Type = image->Type;
		il2ConvertImage(image, image->Format, IL_UNSIGNED_BYTE);
	}


	HPass = Filter(image, filter_h_prewitt, filter_h_prewitt_scale, filter_h_prewitt_bias);
	VPass = Filter(image, filter_v_prewitt, filter_v_prewitt_scale, filter_v_prewitt_bias);
	if (!HPass || !VPass) {
		ifree(HPass);
		ifree(VPass);
		return IL_FALSE;
	}

	// Combine the two passes
	//	Optimization by Matt Denham
	for (i = 0; i < image->SizeOfData; i++) {
		if (HPass[i] == 0)
			image->Data[i] = VPass[i];
		else if (VPass[i] == 0)
			image->Data[i] = HPass[i];
		else
			image->Data[i] = (ILubyte)sqrt((float)(HPass[i]*HPass[i]+VPass[i]*VPass[i]));
	}

	/*for (i = 0; i < image->SizeOfData; i++) {
		image->Data[i] = (ILubyte)sqrt(HPass[i]*HPass[i]+VPass[i]*VPass[i]);
	}*/
	
	ifree(HPass);
	ifree(VPass);

	if (Palette)
		il2ConvertImage(image, IL_COLOUR_INDEX, IL_UNSIGNED_BYTE);
	else if (Converted)
		il2ConvertImage(image, image->Format, Type);

	return IL_TRUE;
}


ILboolean ILAPIENTRY ilu2EdgeDetectS(ILimage* image)
{
	ILubyte		*HPass, *VPass;
	ILuint		i;
	ILboolean	Palette = IL_FALSE, Converted = IL_FALSE;
	ILenum		Type = 0;

	if (image == NULL) {
		il2SetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (image->Format == IL_COLOUR_INDEX) {
		Palette = IL_TRUE;
		il2ConvertImage(image, ilGetPalBaseType(image->Pal.PalType), IL_UNSIGNED_BYTE);
	}
	else if (image->Type > IL_UNSIGNED_BYTE) {
		Converted = IL_TRUE;
		Type = image->Type;
		il2ConvertImage(image, image->Format, IL_UNSIGNED_BYTE);
	}

	HPass = Filter(image, filter_h_sobel, filter_h_sobel_scale, filter_h_sobel_bias);
	VPass = Filter(image, filter_v_sobel, filter_v_sobel_scale, filter_v_sobel_bias);
	if (!HPass || !VPass) {
		ifree(HPass);
		ifree(VPass);
		return IL_FALSE;
	}

	// Combine the two passes
	//	Optimization by Matt Denham
	for (i = 0; i < image->SizeOfData; i++) {
		if (HPass[i] == 0)
			image->Data[i] = VPass[i];
		else if (VPass[i] == 0)
			image->Data[i] = HPass[i];
		else
			image->Data[i] = (ILubyte)sqrt((float)(HPass[i]*HPass[i]+VPass[i]*VPass[i]));
	}

	/*for (i = 0; i < image->SizeOfData; i++) {
		image->Data[i] = (ILubyte)sqrt(HPass[i]*HPass[i]+VPass[i]*VPass[i]);
	}*/
	
	ifree(HPass);
	ifree(VPass);

	if (Palette)
		il2ConvertImage(image, IL_COLOUR_INDEX, IL_UNSIGNED_BYTE);
	else if (Converted)
		il2ConvertImage(image, image->Format, Type);

	return IL_TRUE;
}


ILboolean ILAPIENTRY ilu2BlurAvg(ILimage* image, ILuint Iter)
{
	ILubyte		*Data;
	ILuint		i;
	ILboolean	Palette = IL_FALSE, Converted = IL_FALSE;
	ILenum		Type = 0;

	if (image == NULL) {
		il2SetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (image->Format == IL_COLOUR_INDEX) {
		Palette = IL_TRUE;
		il2ConvertImage(image, ilGetPalBaseType(image->Pal.PalType), IL_UNSIGNED_BYTE);
	}
	else if (image->Type > IL_UNSIGNED_BYTE) {
		Converted = IL_TRUE;
		Type = image->Type;
		il2ConvertImage(image, image->Format, IL_UNSIGNED_BYTE);
	}

	for (i = 0; i < Iter; i++) {
		Data = Filter(image, filter_average, filter_average_scale, filter_average_bias);
		if (!Data)
			return IL_FALSE;
		ifree(image->Data);
		image->Data = Data;
	}

	if (Palette)
		il2ConvertImage(image, IL_COLOUR_INDEX, IL_UNSIGNED_BYTE);
	else if (Converted)
		il2ConvertImage(image, image->Format, Type);

	return IL_TRUE;
}


ILboolean ILAPIENTRY ilu2BlurGaussian(ILimage* image, ILuint Iter)
{
	ILubyte		*Data;
	ILuint		i;
	ILboolean	Palette = IL_FALSE, Converted = IL_FALSE;
	ILenum		Type = 0;

	if (image == NULL) {
		il2SetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (image->Format == IL_COLOUR_INDEX) {
		Palette = IL_TRUE;
		il2ConvertImage(image, ilGetPalBaseType(image->Pal.PalType), IL_UNSIGNED_BYTE);
	}
	else if (image->Type > IL_UNSIGNED_BYTE) {
		Converted = IL_TRUE;
		Type = image->Type;
		il2ConvertImage(image, image->Format, IL_UNSIGNED_BYTE);
	}

	for (i = 0; i < Iter; i++) {
		Data = Filter(image, filter_gaussian, filter_gaussian_scale, filter_gaussian_bias );
		if (!Data)
			return IL_FALSE;
		ifree(image->Data);
		image->Data = Data;
	}

	if (Palette)
		il2ConvertImage(image, IL_COLOUR_INDEX, IL_UNSIGNED_BYTE);
	else if (Converted)
		il2ConvertImage(image, image->Format, Type);

	return IL_TRUE;
}


ILboolean ILAPIENTRY ilu2Emboss(ILimage* image)
{
	ILubyte		*Data;
	ILboolean	Palette = IL_FALSE, Converted = IL_FALSE;
	ILenum		Type = 0;

	if (image == NULL) {
		il2SetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (image->Format == IL_COLOUR_INDEX) {
		Palette = IL_TRUE;
		il2ConvertImage(image, ilGetPalBaseType(image->Pal.PalType), IL_UNSIGNED_BYTE);
	}
	else if (image->Type > IL_UNSIGNED_BYTE) {
		Converted = IL_TRUE;
		Type = image->Type;
		il2ConvertImage(image, image->Format, IL_UNSIGNED_BYTE);
	}

	Data = Filter(image, filter_emboss, filter_emboss_scale, filter_emboss_bias);
	if (!Data)
		return IL_FALSE;
	ifree(image->Data);
	image->Data = Data;

	if (Palette)
		il2ConvertImage(image, IL_COLOUR_INDEX, IL_UNSIGNED_BYTE);
	else if (Converted)
		il2ConvertImage(image, image->Format, Type);

	return IL_TRUE;
}


ILboolean ILAPIENTRY ilu2EdgeDetectE(ILimage* image)
{
	ILubyte		*Data;
	ILboolean	Palette = IL_FALSE, Converted = IL_FALSE;
	ILenum		Type = 0;

	if (image == NULL) {
		il2SetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if (image->Format == IL_COLOUR_INDEX) {
		Palette = IL_TRUE;
		il2ConvertImage(image, ilGetPalBaseType(image->Pal.PalType), IL_UNSIGNED_BYTE);
	}
	else if (image->Type > IL_UNSIGNED_BYTE) {
		Converted = IL_TRUE;
		Type = image->Type;
		il2ConvertImage(image, image->Format, IL_UNSIGNED_BYTE);
	}

	Data = Filter(image, filter_embossedge, filter_embossedge_scale, filter_embossedge_bias);
	if (!Data)
		return IL_FALSE;
	ifree(image->Data);
	image->Data = Data;

	if (Palette)
		il2ConvertImage(image, IL_COLOUR_INDEX, IL_UNSIGNED_BYTE);
	else if (Converted)
		il2ConvertImage(image, image->Format, Type);

	return IL_TRUE;
}

ILboolean ILAPIENTRY ilu2ScaleAlpha(ILimage* image, ILfloat scale)
{
	ILuint		i;
	ILint		alpha;

	if (image == NULL) {
		il2SetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if( (image->Format != IL_COLOUR_INDEX) && (image->Type != IL_BYTE) ) {
		il2SetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	ILint paletteBPP;
	il2GetImageInteger(image, IL_PALETTE_BPP, &paletteBPP);

	switch (image->Format)
	{
		case IL_BGRA:
		case IL_RGBA:
			for (i = 0; i < image->SizeOfData; i += image->Bpp) {
				alpha = (ILint)(image->Data[i+3] * scale);
				if (alpha > UCHAR_MAX) alpha = UCHAR_MAX;
				if (alpha < 0) alpha = 0;
				image->Data[i+3] = (ILubyte)alpha;
			}
			break;

		case IL_COLOUR_INDEX:
			switch (image->Pal.PalType)
			{
				case IL_PAL_RGBA32:
				case IL_PAL_BGRA32:
					for (i = 0; i < image->Pal.PalSize; i += paletteBPP) {
						alpha = (ILint)(image->Pal.Palette[i+3] * scale);
						if (alpha > UCHAR_MAX) alpha = UCHAR_MAX;
						if (alpha < 0) alpha = 0;
						image->Pal.Palette[i+3] = (ILubyte)alpha;
					}
					break;

				default:
					il2SetError(ILU_ILLEGAL_OPERATION);
					return IL_FALSE;
			}
			break;

		default:
			il2SetError(ILU_ILLEGAL_OPERATION);
			return IL_FALSE;
	}

	return IL_TRUE;
}

// Everything here was taken from "Grafica Obscura",
//	found at http://www.sgi.com/grafica/matrix/index.html


// @TODO:  fast float to byte
//! Scales image colours
ILboolean ILAPIENTRY ilu2ScaleColours(ILimage* image, ILfloat r, ILfloat g, ILfloat b) {
	ILuint		i;
	ILint		red, grn, blu, grey;
	ILushort	*ShortPtr;
	ILuint		*IntPtr, NumPix;
	ILdouble	*DblPtr;

	if (image == NULL) {
		il2SetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	if( (image->Format != IL_COLOUR_INDEX) && (image->Type != IL_BYTE) ) {
		il2SetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	ILint paletteBPP;
	il2GetImageInteger(image, IL_PALETTE_BPP, &paletteBPP);

	switch (image->Format)
	{
		case IL_RGB:
		case IL_RGBA:
			for (i = 0; i < image->SizeOfData; i += image->Bpp) {
				red = (ILint)(image->Data[i] * r);
				grn = (ILint)(image->Data[i+1] * g);
				blu = (ILint)(image->Data[i+2] * b);
				if (red > UCHAR_MAX) red = UCHAR_MAX;
				if (red < 0) red = 0;
				if (grn > UCHAR_MAX) grn = UCHAR_MAX;
				if (grn < 0) grn = 0;
				if (blu > UCHAR_MAX) blu = UCHAR_MAX;
				if (blu < 0) blu = 0;
				image->Data[i]   = (ILubyte)red;
				image->Data[i+1] = (ILubyte)grn;
				image->Data[i+2] = (ILubyte)blu;
			}
			break;

		case IL_BGR:
		case IL_BGRA:
			for (i = 0; i < image->SizeOfData; i += image->Bpp) {
				red = (ILint)(image->Data[i+2] * r);
				grn = (ILint)(image->Data[i+1] * g);
				blu = (ILint)(image->Data[i] * b);
				if (red > UCHAR_MAX) red = UCHAR_MAX;
				if (red < 0) red = 0;
				if (grn > UCHAR_MAX) grn = UCHAR_MAX;
				if (grn < 0) grn = 0;
				if (blu > UCHAR_MAX) blu = UCHAR_MAX;
				if (blu < 0) blu = 0;
				image->Data[i+2] = (ILubyte)red;
				image->Data[i+1] = (ILubyte)grn;
				image->Data[i]   = (ILubyte)blu;
			}
			break;

		case IL_LUMINANCE:
		case IL_LUMINANCE_ALPHA:
			NumPix = image->SizeOfData / (image->Bpc*image->Bpp);
			switch (image->Bpc) {
				case 1:
					for (i = 0; i < NumPix; i+=image->Bpp) {
						grey = (ILint)(image->Data[i] * r);
						if (grey > UCHAR_MAX) grey = UCHAR_MAX;
						if (grey < 0) grey = 0;
						image->Data[i] = (ILubyte)grey;
					}
					break;

				case 2:
					ShortPtr = (ILushort*)image->Data;
					for (i = 0; i < NumPix; i+=image->Bpp) {
						grey = (ILint)(ShortPtr[i] * r);
						if (grey > USHRT_MAX) grey = USHRT_MAX;
						if (grey < 0) grey = 0;
						ShortPtr[i] = (ILushort)grey;
					}
					break;

				case 4:
					IntPtr = (ILuint*)image->Data;
					for (i = 0; i < NumPix; i+=image->Bpp) {
						grey = (ILint)(IntPtr[i] * r);
						if (grey < 0) grey = 0;
						IntPtr[i] = (ILuint)grey;
					}
					break;

				case 8:
					DblPtr = (ILdouble*)image->Data;
					for (i = 0; i < NumPix; i+=image->Bpp) {
						DblPtr[i] = DblPtr[i] * r;
					}
					break;
			}
			break;

		case IL_COLOUR_INDEX:
			switch (image->Pal.PalType)
			{
				case IL_PAL_RGB24:
				case IL_PAL_RGB32:
				case IL_PAL_RGBA32:
					for (i = 0; i < image->Pal.PalSize; i += paletteBPP) {
						red = (ILint)(image->Pal.Palette[i] * r);
						grn = (ILint)(image->Pal.Palette[i+1] * g);
						blu = (ILint)(image->Pal.Palette[i+2] * b);
						if (red > UCHAR_MAX) red = UCHAR_MAX;
						if (red < 0) red = 0;
						if (grn > UCHAR_MAX) grn = UCHAR_MAX;
						if (grn < 0) grn = 0;
						if (blu > UCHAR_MAX) blu = UCHAR_MAX;
						if (blu < 0) blu = 0;
						image->Pal.Palette[i]   = (ILubyte)red;
						image->Pal.Palette[i+1] = (ILubyte)grn;
						image->Pal.Palette[i+2] = (ILubyte)blu;
					}
					break;

				case IL_PAL_BGR24:
				case IL_PAL_BGR32:
				case IL_PAL_BGRA32:
					for (i = 0; i < image->Pal.PalSize; i += paletteBPP) {
						red = (ILint)(image->Pal.Palette[i+2] * r);
						grn = (ILint)(image->Pal.Palette[i+1] * g);
						blu = (ILint)(image->Pal.Palette[i] * b);
						if (red > UCHAR_MAX) red = UCHAR_MAX;
						if (red < 0) red = 0;
						if (grn > UCHAR_MAX) grn = UCHAR_MAX;
						if (grn < 0) grn = 0;
						if (blu > UCHAR_MAX) blu = UCHAR_MAX;
						if (blu < 0) blu = 0;
						image->Pal.Palette[i+2]   = (ILubyte)red;
						image->Pal.Palette[i+1] = (ILubyte)grn;
						image->Pal.Palette[i] = (ILubyte)blu;
					}
					break;


				default:
					il2SetError(ILU_ILLEGAL_OPERATION);
					return IL_FALSE;
			}
			break;

		default:
			il2SetError(ILU_ILLEGAL_OPERATION);
			return IL_FALSE;
	}

	return IL_TRUE;
}


ILboolean ILAPIENTRY ilu2GammaCorrect(ILimage* image, ILfloat Gamma)
{
	ILfloat		Table[256];
	ILuint		i, NumPix;
	ILushort	*ShortPtr;
	ILuint		*IntPtr;

	if (image == NULL) {
		il2SetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	for (i = 0; i < 256; i++) {
		Table[i] = (ILfloat)pow(i / 255.0, 1.0 / Gamma);
	}

	// Do we need to clamp here?
	if (image->Format == IL_COLOUR_INDEX) {
		for (i = 0; i < image->Pal.PalSize; i++) {
			image->Pal.Palette[i] = (ILubyte)(Table[image->Pal.Palette[i]] * 255);
		}
	}
	else {
		// Not too sure if this is the correct way of handling multiple bpc's.
		switch (image->Bpc)
		{
			case 1:
				for (i = 0; i < image->SizeOfData; i++) {
					image->Data[i] = (ILubyte)(Table[image->Data[i]] * UCHAR_MAX);
				}
				break;
			case 2:
				NumPix = image->SizeOfData / 2;
				ShortPtr = (ILushort*)image->Data;
				for (i = 0; i < NumPix; i++) {
					ShortPtr[i] = (ILushort)(Table[ShortPtr[i] >> 8] * USHRT_MAX);
				}
				break;
			case 4:
				NumPix = image->SizeOfData / 4;
				IntPtr = (ILuint*)image->Data;
				for (i = 0; i < NumPix; i++) {
					IntPtr[i] = (ILuint)(Table[IntPtr[i] >> 24] * UINT_MAX);
				}
				break;
		}
	}

	return IL_TRUE;
}


//
// Colour matrix stuff follows
// ---------------------------
//


void iApplyMatrix(ILimage *Image, ILfloat Mat[4][4])
{
	ILubyte	*Data = Image->Data;
	ILuint	i;
	ILubyte	r, g, b;

	switch (Image->Format)
	{
		case IL_RGB:
		case IL_RGBA:
			for (i = 0; i < Image->SizeOfData; ) {
				r = (ILubyte)(Data[i] * Mat[0][0] + Data[i+1] * Mat[1][0] +
									Data[i+2] * Mat[2][0]);// + Mat[3][0]);
				g = (ILubyte)(Data[i] * Mat[0][1] + Data[i+1] * Mat[1][1] +
									Data[i+2] * Mat[2][1]);// + Mat[3][1]);
				b = (ILubyte)(Data[i] * Mat[0][2] + Data[i+1] * Mat[1][2] +
									Data[i+2] * Mat[2][2]);// + Mat[3][2]);
				Data[i]   = r;
				Data[i+1] = g;
				Data[i+2] = b;
				i += Image->Bpp;
			}
			break;

		case IL_BGR:
		case IL_BGRA:
			for (i = 0; i < Image->SizeOfData; ) {
				r = (ILubyte)(Data[i] * Mat[0][2] + Data[i+1] * Mat[1][2] +
									Data[i+2] * Mat[2][2]);// + Mat[3][2]);
				g = (ILubyte)(Data[i] * Mat[0][1] + Data[i+1] * Mat[1][1] +
									Data[i+2] * Mat[2][1]);// + Mat[3][1]);
				b = (ILubyte)(Data[i] * Mat[0][0] + Data[i+1] * Mat[1][0] +
									Data[i+2] * Mat[2][0]);// + Mat[3][0]);
				Data[i]   = b;
				Data[i+1] = g;
				Data[i+2] = r;
				i += Image->Bpp;
			}
			break;

		default:
			il2SetError(ILU_ILLEGAL_OPERATION);
			return;
	}

	return;
}


void iIdentity(ILfloat *Matrix)
{
    *Matrix++ = 1.0;    // row 1
    *Matrix++ = 0.0;
    *Matrix++ = 0.0;
    //*Matrix++ = 0.0;
    *Matrix++ = 0.0;    // row 2
    *Matrix++ = 1.0;
    *Matrix++ = 0.0;
    //*Matrix++ = 0.0;
    *Matrix++ = 0.0;    // row 3
    *Matrix++ = 0.0;
    *Matrix++ = 1.0;
    //*Matrix++ = 0.0;
    /**Matrix++ = 0.0;    // row 4
    *Matrix++ = 0.0;
    *Matrix++ = 0.0;
    *Matrix++ = 1.0;*/
}


ILboolean ILAPIENTRY ilu2Saturate4f(ILimage* image, ILfloat r, ILfloat g, ILfloat b, ILfloat Saturation)
{
	ILfloat Mat[4][4];
	ILfloat s = Saturation;

	Mat[0][0] = (1.0f - s) * r + s;
	Mat[0][1] = (1.0f - s) * r;
	Mat[0][2] = (1.0f - s) * r;
	Mat[0][3] = 0.0f;

	Mat[1][0] = (1.0f - s) * g;
	Mat[1][1] = (1.0f - s) * g + s;
	Mat[1][2] = (1.0f - s) * g;
	Mat[1][3] = 0.0f;

	Mat[2][0] = (1.0f - s) * b;
	Mat[2][1] = (1.0f - s) * b;
	Mat[2][2] = (1.0f - s) * b + s;
	Mat[2][3] = 0.0f;

	Mat[3][0] = 0.0f;
	Mat[3][1] = 0.0f;
	Mat[3][2] = 0.0f;
	Mat[3][3] = 1.0f;

	if (image == NULL) {
		il2SetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	iApplyMatrix(image, Mat);

	return IL_TRUE;
}


ILboolean ILAPIENTRY ilu2Saturate1f(ILimage* image, ILfloat Saturation)
{
	return ilu2Saturate4f(image, 0.3086f, 0.6094f, 0.0820f, Saturation);
}


//! Funny as hell filter that I stumbled upon accidentally
ILboolean ILAPIENTRY ilu2Alienify(ILimage* image)
{
	ILfloat	Mat[3][3];
	ILubyte	*Data;
	ILuint	i;

	if (image == NULL) {
		il2SetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}
	Data = image->Data;

	iIdentity((ILfloat*)Mat);

	switch (image->Format)
	{
		case IL_BGR:
		case IL_BGRA:
			for (i = 0; i < image->SizeOfData; i += image->Bpp) {
				Data[i+2] = (ILubyte)(Data[i] * Mat[0][0] + Data[i+1] * Mat[1][0] +
									Data[i+2] * Mat[2][0]);// + Mat[3][0]);
				Data[i+1] = (ILubyte)(Data[i] * Mat[0][1] + Data[i+1] * Mat[1][1] +
									Data[i+2] * Mat[2][1]);// + Mat[3][1]);
				Data[i]   = (ILubyte)(Data[i] * Mat[0][2] + Data[i+1] * Mat[1][2] +
									Data[i+2] * Mat[2][2]);// + Mat[3][2]);
			}
		break;

		case IL_RGB:
		case IL_RGBA:
			for (i = 0; i < image->SizeOfData; i += image->Bpp) {
				Data[i]   = (ILubyte)(Data[i] * Mat[0][2] + Data[i+1] * Mat[1][2] +
									Data[i+2] * Mat[2][2]);// + Mat[3][2]);
				Data[i+1] = (ILubyte)(Data[i] * Mat[0][1] + Data[i+1] * Mat[1][1] +
									Data[i+2] * Mat[2][1]);// + Mat[3][1]);
				Data[i+2] = (ILubyte)(Data[i] * Mat[0][0] + Data[i+1] * Mat[1][0] +
									Data[i+2] * Mat[2][0]);// + Mat[3][0]);
			}
		break;

		default:
			//il2SetError(ILU_ILLEGAL_OPERATION);
			return IL_FALSE;
	}

	return IL_TRUE;
}




// Everything here was taken from
//	http://bae.fse.missouri.edu/luw/course/image/project1/project1.html
//	and http://www.sgi.com/grafica/interp/index.html



void iIntExtImg(ILimage *Image1, ILimage *Image2, ILfloat a)
{
	ILuint	i;
	ILint	d;
	ILubyte	*Data1, *Data2;

	Data1 = Image1->Data;
	Data2 = Image2->Data;

	for (i = 0; i < Image2->SizeOfData; i++) {
		d = (ILint)((1 - a) * *Data1 + a * *Data2);
		// Limit a pixel value to the range [0, 255]
		if (d < 0) 
			d = 0;
		else if (d > 255)
			d = 255;
		*Data2 = d;

		Data1++;
		Data2++;
	}
			
	return;
}


// Works best when Contrast is in the range [-0.5, 1.7].
ILboolean ILAPIENTRY ilu2Contrast(ILimage* image, ILfloat Contrast)
{
	ILimage	*Grey;
	ILuint	i;

	if (image == NULL) {
		il2SetError(IL_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	Grey = il2NewImage(image->Width, image->Height, image->Depth, image->Bpp, image->Bpc);
	if (Grey == NULL) {
		return IL_FALSE;
	}

	// Make Grey all 127's
	for (i = 0; i < Grey->SizeOfData; i++) {
		Grey->Data[i] = 127;
	}

	iIntExtImg(Grey, image, Contrast);
	ilCloseImage(Grey);

	return IL_TRUE;
}


// Sharpens when Factor is in the range [1.0, 2.5].
// Blurs when Factor is in the range [0.0, 1.0].
ILboolean ILAPIENTRY ilu2Sharpen(ILimage* image, ILfloat Factor, ILuint Iter)
{
	ILimage	*Blur;  // iluBlur() changes image
	ILuint	i;
	
	if (image == NULL) {
		il2SetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}

	Blur = il2NewImage(image->Width, image->Height, image->Depth, image->Bpp, image->Bpc);
	if (Blur == NULL) {
		return IL_FALSE;
	}
	il2CopyImageAttr(Blur, image);

	il2CopyPixels(image, 0, 0, 0, image->Width, image->Height, 1, image->Format, image->Type, Blur->Data);
	ilSetCurImage(Blur);
	ilu2BlurGaussian(image, 1);

	for (i = 0; i < Iter; i++) {
		iIntExtImg(Blur, image, Factor);
	}

	ilCloseImage(Blur);
	ilSetCurImage(image);

	return IL_TRUE;
}


ILAPI ILboolean ILAPIENTRY ilu2Convolution(ILimage* image, ILint *matrix, ILint scale, ILint bias) 
{
	ILubyte		*Data;
	ILboolean	Palette = IL_FALSE, Converted = IL_FALSE;
	ILenum		Type = 0;
	
	if (image == NULL) {
		il2SetError(ILU_ILLEGAL_OPERATION);
		return IL_FALSE;
	}
	
	if (image->Format == IL_COLOUR_INDEX) {
		Palette = IL_TRUE;
		il2ConvertImage(image, ilGetPalBaseType(image->Pal.PalType), IL_UNSIGNED_BYTE);
	} else if (image->Type > IL_UNSIGNED_BYTE) {
		Converted = IL_TRUE;
		Type = image->Type;
		il2ConvertImage(image, image->Format, IL_UNSIGNED_BYTE);
	}
	
	Data = Filter(image, matrix, scale, bias);
	if (!Data)
		return IL_FALSE;
	ifree(image->Data);
	image->Data = Data;
	
	if (Palette)
		il2ConvertImage(image, IL_COLOUR_INDEX, IL_UNSIGNED_BYTE);
	else if (Converted)
		il2ConvertImage(image, image->Format, Type);
	
	return IL_TRUE;
}

