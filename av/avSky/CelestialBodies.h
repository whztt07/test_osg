/* -*-c++-*-  */

/**************************************************************************
 * Written by Durk Talsma. Originally started October 1997, for distribution  
 * with the FlightGear project. Version 2 was written in August and 
 * September 1998. This code is based upon algorithms and data kindly 
 * provided by Mr. Paul Schlyter. (pausch@saaf.se). 
 *
 * This code has been repackaged for use with avSky by Don Burns
 * November 25, 2005 by placing all celestial body classes into one
 * header file and one source file.  
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA  02111-1307, USA.
 *
 **************************************************************************/
#ifndef OSGEPHEMERIS_CELESTIAL_BODIES_DEF
#define OSGEPHEMERIS_CELESTIAL_BODIES_DEF

#include <osg/Referenced>

namespace avSky {

class Sun;

/** \class CelestialBody
    \brief A super class for all celestial bodies - Used Internally.

    */
class CelestialBody: public osg::Referenced
{
    public:
        CelestialBody(double Nf, double Ns,
            double If, double Is,
            double wf, double ws,
            double af, double as,
            double ef, double es,
            double Mf, double Ms, double mjd);
        CelestialBody(double Nf, double Ns,
            double If, double Is,
            double wf, double ws,
            double af, double as,
            double ef, double es,
            double Mf, double Ms);

        void getPos(double *ra, double *dec) const;
        void getPos(double *ra, double *dec, double *magnitude) const;
        double getRightAscension() const  { return rightAscension; }
        double getDeclination() const    { return declination; }
        double getMagnitude() const      { return magnitude; }
        double getLon() const            { return lonEcl; }
        double getLat() const            { return latEcl; }
        void updatePosition(double mjd, Sun *ourSun);

    protected:              // make the data protected, in order to give the
                            //    inherited classes direct access to the data
        double NFirst;      // longitude of the ascending node first part 
        double NSec;        // longitude of the ascending node second part
        double iFirst;      // inclination to the ecliptic first part 
        double iSec;        // inclination to the ecliptic second part
        double wFirst;      // first part of argument of perihelion 
        double wSec;        // second part of argument of perihelion 
        double aFirst;      // semimayor axis first part
        double aSec;        // semimayor axis second part
        double eFirst;      // eccentricity first part 
        double eSec;        // eccentricity second part
        double MFirst;      // Mean anomaly first part 
        double MSec;        // Mean anomaly second part

        double N, i, w, a, e, M; // the resulting orbital elements, obtained from the former 

        double rightAscension, declination;
        double r, R, s, FV;
        double magnitude;
        double lonEcl, latEcl;

        double sgCalcEccAnom(double M, double e);
        double sgCalcActTime(double mjd);
        void updateOrbElements(double mjd);
};

// \class Sun
// \brief The Sun celestial body - used internally
class Sun : public CelestialBody
{
    public:
        Sun (double mjd);
        Sun ();
        void updatePosition(double mjd);
        double getM() { return M; }
        double getw() { return w; }
        double getxs() { return xs; }
        double getys() { return ys; }
        double getDistance() { return distance; }

    protected:
        virtual ~Sun();

    private:

        double xs, ys;       // the sun's rectangular geocentric coordinates
        double distance;     // the sun's distance to the earth
};

// \class Moon
// \brief The Moon celestial body - used internally
class Moon : public CelestialBody
{
    public:
        Moon(double mjd);
        Moon();
        void updatePosition(double mjd, double lst, double lat, Sun *ourSun);

    protected:
        virtual ~Moon();
};


}
#endif





