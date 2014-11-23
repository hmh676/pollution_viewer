
#ifndef ISOLINES_H
#define ISOLINES_H

#include "utils.hpp"
#include "coords.hpp"
#include "lines.hpp"
#include "isolineUtils.hpp"

template <typename F, unsigned int N>
class IsoLineGenerator {
    
    public:
        IsoLineGenerator(const BoundingBox<double> &bbox);

        IsoLine<double,N,F> generateIsoline(const InterpolatedData<F> &data, double isovalue, const Color<N> &color);
        
        IsoLineList<double,N,F> generateIsolines(const InterpolatedData<F> &data, unsigned int nLines, const Colorizer<F,N> &colorizer);

        static void test();

    private:
        static unsigned char computeCase(double d[], double isovalue);
        static void attachLine(Line<double> &line, MultiLine<double> &lineList);

        const BoundingBox<double> bbox;
};

#include "isolineGenerator.tpp"

#endif /* end of include guard: ISOLINES_H */
