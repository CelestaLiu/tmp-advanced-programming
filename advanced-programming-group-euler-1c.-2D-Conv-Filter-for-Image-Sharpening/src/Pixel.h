/**
 * @file Pixel.h
 * @brief Declarations of the Pixel class for image processing
 * @group [Euler]
 * 
 * Group members:
 * - [Davide Baino] ([esemsc-db24])
 * - [Qi Gao] ([esemsc-qg124])
 * - [Daniel Kaupa] ([esemsc-dbk24])
 * - [Leyao Liu] ([esemsc-ll1524])
 * - [Jiayi Lu] ([esemsc-jl7324])
 * - [Ananya Sinha] ([esemsc-as10524])
 * - [Mingwei Yan] ([esemsc-my324])
 */

#ifndef PIXEL_H
#define PIXEL_H

/**
 * @brief Class representing a single pixel in an image
 * 
 * The Pixel class encapsulates RGB(A) color values and provides methods for
 * color space conversions and manipulation. This is the fundamental building block
 * for all image processing operations in the project.
 */
class Pixel {
private:
    unsigned char r; ///< Red channel value (0-255)
    unsigned char g; ///< Green channel value (0-255)
    unsigned char b; ///< Blue channel value (0-255)
    unsigned char a; ///< Alpha channel value (0-255), default 255 (opaque)

public:
   // *******************************************************************************************
   // -------------------------------------------------------------------------------------------
   // CONSTRUCTORS, DESTRUCTOR & CLONE
   // -------------------------------------------------------------------------------------------
   // *******************************************************************************************

    /**
     * @brief Default constructor, creates a black opaque pixel
     */
    Pixel();

    /**
     * @brief Constructor with RGB values, alpha defaults to 255 (opaque)
     * 
     * @param r Red channel value (0-255)
     * @param g Green channel value (0-255)
     * @param b Blue channel value (0-255)
     * @param a Alpha channel value (0-255), defaults to 255
     * // default to 255 because it maintains backward compatibility as most images are opaque by default (if not specified)
     */
    Pixel(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);

    /**
     * @brief Copy constructor
     * 
     * @param other The pixel to copy from
     */
    Pixel(const Pixel& other);

         /**
     * @brief Destructor
     */
    ~Pixel();

   // *******************************************************************************************
   // -------------------------------------------------------------------------------------------
   // GETTERS & SETTERS
   // -------------------------------------------------------------------------------------------
   // *******************************************************************************************

    /**
     * @brief Get the red channel value
     * 
     * @return unsigned char Red value (0-255)
     */
    unsigned char getR() const;

    /**
     * @brief Get the green channel value
     * 
     * @return unsigned char Green value (0-255)
     */
    unsigned char getG() const;

    /**
     * @brief Get the blue channel value
     * 
     * @return unsigned char Blue value (0-255)
     */
    unsigned char getB() const;

    /**
     * @brief Get the alpha channel value
     * 
     * @return unsigned char Alpha value (0-255)
     */
    unsigned char getA() const;

    /**
     * @brief Set the red channel value
     * 
     * @param r Red value (0-255)
     */
    void setR(unsigned char r);

    /**
     * @brief Set the green channel value
     * 
     * @param g Green value (0-255)
     */
    void setG(unsigned char g);

    /**
     * @brief Set the blue channel value
     * 
     * @param b Blue value (0-255)
     */
    void setB(unsigned char b);

    /**
     * @brief Set the alpha channel value
     * 
     * @param a Alpha value (0-255)
     */
    void setA(unsigned char a);

    /**
     * @brief Set all RGB(A) values at once
     * 
     * @param r Red value (0-255)
     * @param g Green value (0-255)
     * @param b Blue value (0-255)
     * @param a Alpha value (0-255), defaults to 255
     */
    void setRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);

    /**
     * @brief Convert the pixel to grayscale using the luminance formula
     * 
     * Uses the formula Y = 0.2126R + 0.7152G + 0.0722B as specified in the project requirements
     * 
     * @return Pixel A new grayscale pixel where R = G = B = Y
     */

   /**
     * @brief Calculate the intensity/luminance value of the pixel
     * 
     * Uses the formula Y = 0.2126R + 0.7152G + 0.0722B
     * 
     * @return float The luminance value (0-255)
     */
    float getLuminance() const;


   // *******************************************************************************************
   // -------------------------------------------------------------------------------------------
   // IMAGE MANIPULATION
   // -------------------------------------------------------------------------------------------
   // *******************************************************************************************


    Pixel toGrayscale() const;

    /**
     * @brief Convert RGB to HSV color space
     * 
     * @param h Reference to store hue (0-360)
     * @param s Reference to store saturation (0-1)
     * @param v Reference to store value (0-1)
     */
    void RGBtoHSV(float& h, float& s, float& v) const;

    /**
     * @brief Set pixel values from HSV color space (convert back to RGB)
     * 
     * @param h Hue (0-360)
     * @param s Saturation (0-1)
     * @param v Value (0-1)
     */
    void HSVtoRGB(float h, float s, float v);
   
    /**
     * @brief Convert RGB to HSL color space
     * 
     * @param h Reference to store hue (0-360)
     * @param s Reference to store saturation (0-1)
     * @param l Reference to store lightness (0-1)
     */
    void RGBtoHSL(float& h, float& s, float& l) const;

    /**
     * @brief Set pixel values from HSL color space (convert back to RGB)
     * 
     * @param h Hue (0-360)
     * @param s Saturation (0-1)
     * @param l Lightness (0-1)
     */
    void HSLtoRGB(float h, float s, float l);

    /**
     * @brief Add a value to all color channels (for brightness adjustment)
     * 
     * @param value Value to add (-255 to 255)
     * @return Pixel A new pixel with adjusted brightness
     */
    Pixel adjustBrightness(int value) const;

    // *******************************************************************************************
    // -------------------------------------------------------------------------------------------
    // OPERATOR OVERRIDES 
    // -------------------------------------------------------------------------------------------
    // *******************************************************************************************
    /**
     * @brief Assignment operator
     * 
     * @param other The pixel to assign from
     * @return Reference to this pixel after assignment
     */
    Pixel& operator=(const Pixel& other);


    /**
     * @brief Check if two pixels are equal (same RGBA values)
     * 
     * @param other The pixel to compare with
     * @return true If all RGBA values are equal
     * @return false Otherwise
     */
    bool operator==(const Pixel& other) const;

    /**
     * @brief Check if two pixels are not equal
     * 
     * @param other The pixel to compare with
     * @return true If any RGBA value differs
     * @return false If all RGBA values are equal
     */
    bool operator!=(const Pixel& other) const;
};

#endif // PIXEL_H