
#ifndef COLORIZER_H
#define COLORIZER_H

#include <type_traits>
#include <cassert>
#include <algorithm>
#include <iomanip>   

#include "interpolator.hpp"
#include "colors.hpp"
#include "image.hpp"
#include "stringBlitter.hpp"

template <typename F, unsigned int N>
class Colorizer {
    static_assert(std::is_floating_point<F>(), "F must be floating point !");

    public:
        virtual ~Colorizer() {}
        virtual Color<N> operator()(const F) const = 0;
   
        void resetMinMax(F min, F max);
        void resetMinMax(const InterpolatedData<F> &data);

        void generateColorRange(const std::string &folder, const std::string &fileName, const std::string &imgExt, const std::string &fontPath) const;

    protected:
        explicit Colorizer() : 
            min(0), max(0) {
        };
        
        F min;
        F max;
};

template <typename F, unsigned int N>
void Colorizer<F,N>::resetMinMax(F min, F max) {
    this->min = min;
    this->max = max;
}
        
template <typename F, unsigned int N>
void Colorizer<F,N>::resetMinMax(const InterpolatedData<F> &data) {
    this->min = data.min;
    this->max = data.max;
}

template <typename F, unsigned int N>
void Colorizer<F,N>::generateColorRange(const std::string &folder, const std::string &fileName, const std::string &imgExt, const std::string &fontPath) const {
    
    using log4cpp::log_console;
    
    StringBlitter blitter;
    blitter.loadFontFromFile(fontPath);
       
    RGBAImageInitializer overlayInit = [&] (unsigned int i, unsigned int j, unsigned int width, unsigned int height) -> Color<N> {
        const unsigned int borderSize = 6u;

        if(i < borderSize || j < borderSize || i+borderSize >= height || j+borderSize >= width)
            return ColorRGBA::blue;
        else 
            return ColorRGBA::white;
    };

    RGBAImageInitializer colorRangeInit = [&] (unsigned int i, unsigned int j, unsigned int width, unsigned int height) -> Color<N> {
        F fy = static_cast<F>(i)/height; 

        const unsigned int borderSize = 3u;
        if(i < borderSize || j < borderSize || i+borderSize >= height || j+borderSize >= width)
            return ColorRGBA::black;

        const F val = min + (F(1)-fy)*(max - min);
        return this->operator()(val);
    };

    Image<4u> overlay(600u,1200u, overlayInit);
    Image<4u> colorRange(400u,1000u, colorRangeInit);
    Image<4u> text;
    
    overlay.blit(colorRange, 40u, 150u);

    blitter.setPixelSize(40u);
    std::stringstream ss;
    StringImageInfo info = blitter.evaluateTextImageSize(ss.str());
    text = blitter.generateTextImageRGBA("Particules PM10 (mg/m³)", ColorRGBA::blue);
    overlay.blit(text, 30, 30 - info.imgHeight/3);         
    text.freeData();
  
    blitter.setPixelSize(50u);
    unsigned int nLevels = 5;
    for (unsigned int i = 0; i < nLevels; i++) {
        ss.seekp(std::ios_base::beg);
        F alpha = static_cast<F>(i)/(nLevels-1);
        F val = min + (F(1) - alpha)*(max - min);
        ss << std::setprecision(3) << val;
            
        info = blitter.evaluateTextImageSize(ss.str());
        text = blitter.generateTextImageRGBA(ss.str(), ColorRGBA::blue);
        
        overlay.blit(text, 450u, 150+i*1000/(nLevels-1)- info.imgHeight/3);         

        text.freeData();
    }

    overlay.save(folder, fileName, imgExt);

    overlay.freeData();
    colorRange.freeData();
}

#endif /* end of include guard: COLORIZER_H */
