#ifndef WARPX_LaserParticleContainer_H_
#define WARPX_LaserParticleContainer_H_

#include <limits>

#include <WarpXParticleContainer.H>
#include <WarpXConst.H>
#include <WarpXParser.H>

enum class laser_t { Null, Gaussian, Harris, parse_field_function };

class LaserParticleContainer
    : public WarpXParticleContainer
{
public:
    LaserParticleContainer (amrex::AmrCore* amr_core, int ispecies, const std::string& name);
    virtual ~LaserParticleContainer () {}

    virtual void InitData () final;

#ifdef WARPX_DO_ELECTROSTATIC    
    virtual void EvolveES (const amrex::Vector<std::array<std::unique_ptr<amrex::MultiFab>, 3> >& E,
                           amrex::Vector<std::unique_ptr<amrex::MultiFab> >& rho,
                           amrex::Real t, amrex::Real dt) { BL_ASSERT(false); }
#endif // WARPX_DO_ELECTROSTATIC
    
    virtual void Evolve (int lev,
                         const amrex::MultiFab&, const amrex::MultiFab&, const amrex::MultiFab&,
                         const amrex::MultiFab&, const amrex::MultiFab&, const amrex::MultiFab&,
                         amrex::MultiFab& jx, amrex::MultiFab& jy, amrex::MultiFab& jz,
                         amrex::MultiFab*, amrex::MultiFab*, amrex::MultiFab*,
                         amrex::MultiFab* rho, amrex::MultiFab* crho,
                         const amrex::MultiFab*, const amrex::MultiFab*, const amrex::MultiFab*,
                         const amrex::MultiFab*, const amrex::MultiFab*, const amrex::MultiFab*,
                         amrex::Real t, amrex::Real dt) final;

    virtual void PushP (int lev, amrex::Real dt,
                        const amrex::MultiFab& ,
                        const amrex::MultiFab& ,
                        const amrex::MultiFab& ,
                        const amrex::MultiFab& ,
                        const amrex::MultiFab& ,
                        const amrex::MultiFab& ) final;

    virtual void PostRestart () final;

protected:
    
    std::string laser_name;

private:

    // runtime paramters
    laser_t                   profile     = laser_t::Null;
    amrex::Vector<amrex::Real> position;
    amrex::Vector<amrex::Real> nvec;
    amrex::Vector<amrex::Real> p_X;
    amrex::Vector<amrex::Real> stc_direction;

    long                      pusher_algo = -1;
    amrex::Real               e_max       = std::numeric_limits<amrex::Real>::quiet_NaN();
    amrex::Real               wavelength  = std::numeric_limits<amrex::Real>::quiet_NaN();
    amrex::Real               Z0_lab = 0; // Position of the antenna in the lab frame

    // computed using runtime parameters
    amrex::Vector<amrex::Real> p_Y;
    amrex::Vector<amrex::Real> u_X;
    amrex::Vector<amrex::Real> u_Y;
    amrex::Real weight   = std::numeric_limits<amrex::Real>::quiet_NaN();
    amrex::Real mobility = std::numeric_limits<amrex::Real>::quiet_NaN();

    // Gaussian profile
    amrex::Real profile_waist          = std::numeric_limits<amrex::Real>::quiet_NaN();
    amrex::Real profile_duration       = std::numeric_limits<amrex::Real>::quiet_NaN();
    amrex::Real profile_t_peak         = std::numeric_limits<amrex::Real>::quiet_NaN();
    amrex::Real profile_focal_distance = std::numeric_limits<amrex::Real>::quiet_NaN();
    amrex::Real zeta = 0.;
    amrex::Real beta = 0.;
    amrex::Real phi2 = 0.;
    amrex::Real theta_stc = 0.;

    // parse_field_function profile
    WarpXParser parser;
    std::string field_function;

    // laser particle domain
    amrex::RealBox prob_domain;

    void ComputeSpacing (int lev, amrex::Real& Sx, amrex::Real& Sy) const;
    void ComputeWeightMobility (amrex::Real Sx, amrex::Real Sy);
    void InitData (int lev);
};

#endif
