// -*- c++ -*-
#ifndef SBTRANSFORM_IMPL_H
#define SBTRANSFORM_IMPL_H

#include "SBProfileImpl.h"
#include "SBTransform.h"

namespace galsim {

    class SBTransform::SBTransformImpl : public SBProfileImpl
    {
    public:

        SBTransformImpl(const SBProfile& sbin, double mA, double mB, double mC, double mD,
                        const Position<double>& cen, double fluxScaling);

        SBTransformImpl(const SBProfile& sbin, const Ellipse& e, double fluxScaling);

        ~SBTransformImpl() {}

        double xValue(const Position<double>& p) const 
        { return _adaptee.xValue(inv(p-_cen)) * _fluxScaling; }

        std::complex<double> kValue(const Position<double>& k) const;

        bool isAxisymmetric() const { return _stillIsAxisymmetric; }
        bool hasHardEdges() const { return _adaptee.hasHardEdges(); }
        bool isAnalyticX() const { return _adaptee.isAnalyticX(); }
        bool isAnalyticK() const { return _adaptee.isAnalyticK(); }

        double maxK() const { return _adaptee.maxK() / _minor; }
        double stepK() const { return _adaptee.stepK() / _major; }

        void getXRange(double& xmin, double& xmax, std::vector<double>& splits) const;

        void getYRange(double& ymin, double& ymax, std::vector<double>& splits) const;

        void getYRangeX(double x, double& ymin, double& ymax, std::vector<double>& splits) const;

        Position<double> centroid() const { return _cen+fwd(_adaptee.centroid()); }

        double getFlux() const { return _adaptee.getFlux()*_absdet; }

        double getPositiveFlux() const 
        { return _adaptee.getPositiveFlux()*_absdet; }
        double getNegativeFlux() const 
        { return _adaptee.getNegativeFlux()*_absdet; }

        /**
         * @brief Shoot photons through this SBTransform.
         *
         * SBTransform will simply apply the affine transformation to coordinates of photons
         * generated by its adaptee, and rescale the flux by the determinant of the distortion
         * matrix.
         * @param[in] N Total number of photons to produce.
         * @param[in] ud UniformDeviate that will be used to draw photons from distribution.
         * @returns PhotonArray containing all the photons' info.
         */
        boost::shared_ptr<PhotonArray> shoot(int N, UniformDeviate ud) const;

        // Override for better efficiency:
        void fillKGrid(KTable& kt) const; 

    private:
        SBProfile _adaptee; ///< SBProfile being adapted/transformed

        double _mA; ///< A element of 2x2 distortion matrix `M = [(A B), (C D)]` = [row1, row2]
        double _mB; ///< B element of 2x2 distortion matrix `M = [(A B), (C D)]` = [row1, row2]
        double _mC; ///< C element of 2x2 distortion matrix `M = [(A B), (C D)]` = [row1, row2]
        double _mD; ///< D element of 2x2 distortion matrix `M = [(A B), (C D)]` = [row1, row2]
        Position<double> _cen;  ///< Centroid position.

        // Calculate and save these:
        double _absdet;  ///< Determinant (flux magnification) of `M` matrix * fluxScaling
        double _fluxScaling;  ///< Amount to multiply flux by.
        double _invdet;  ///< Inverse determinant of `M` matrix.
        double _major; ///< Major axis of ellipse produced from unit circle.
        double _minor; ///< Minor axis of ellipse produced from unit circle.
        bool _stillIsAxisymmetric; ///< Is output SBProfile shape still circular?
        double _xmin, _xmax, _ymin, _ymax; ///< Ranges propagated from adaptee
        double _coeff_b, _coeff_c, _coeff_c2; ///< Values used in getYRangeX(x,ymin,ymax);
        std::vector<double> _xsplits, _ysplits; ///< Good split points for the intetegrals

        void initialize();

        /** 
         * @brief Forward coordinate transform with `M` matrix.
         *
         * @param[in] p input position.
         * @returns transformed position.
         */
        Position<double> fwd(const Position<double>& p) const 
        { return _fwd(_mA,_mB,_mC,_mD,p.x,p.y,_invdet); }

        /// @brief Forward coordinate transform with transpose of `M` matrix.
        Position<double> fwdT(const Position<double>& p) const 
        { return _fwd(_mA,_mC,_mB,_mD,p.x,p.y,_invdet); }

        /// @brief Inverse coordinate transform with `M` matrix.
        Position<double> inv(const Position<double>& p) const 
        { return _inv(_mA,_mB,_mC,_mD,p.x,p.y,_invdet); }

        /// @brief Returns the k value (no phase).
        std::complex<double> kValueNoPhase(const Position<double>& k) const;

        std::complex<double> (*_kValue)(
            const SBProfile& adaptee, const Position<double>& fwdTk, double absdet,
            const Position<double>& k, const Position<double>& cen);
        std::complex<double> (*_kValueNoPhase)(
            const SBProfile& adaptee, const Position<double>& fwdTk, double absdet,
            const Position<double>& , const Position<double>& );

        Position<double> (*_fwd)(
            double mA, double mB, double mC, double mD, double x, double y, double );
        Position<double> (*_inv)(
            double mA, double mB, double mC, double mD, double x, double y, double invdet);

        // Copy constructor and op= are undefined.
        SBTransformImpl(const SBTransformImpl& rhs);
        void operator=(const SBTransformImpl& rhs);
    };
}

#endif // SBTRANSFORM_IMPL_H
