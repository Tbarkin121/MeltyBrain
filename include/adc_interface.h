/*------------------------------------------------------------------------------
* Copyright © Portal Instruments – All Rights Reserved
* Proprietary and Confidential
* Copying of this file or use of code contained in this file via any
* medium is strictly prohibited unless prior written permission is obtained
* from Portal Instruments, Inc.
* Created by Adam Casey 2016
------------------------------------------------------------------------------*/

#ifndef INCLUDE_ADC_INTERFACE_H_
#define INCLUDE_ADC_INTERFACE_H_

#include <boost/units/base_unit.hpp>
#include <boost/units/quantity.hpp>
#include <boost/units/systems/si/electric_potential.hpp>
#include <system_error>  // NOLINT(build/c++11)

using namespace boost::units;  // NOLINT(build/namespaces)
using namespace boost::units::si;  // NOLINT(build/namespaces)

class IAdc {
 public:
    struct AdcReading {
        uint16_t raw_value;
        std::error_code error;
    };

    virtual ~IAdc() {}
    virtual std::error_code Initialize() const = 0;
    virtual AdcReading Read() const = 0;
    virtual quantity<electric_potential, float> GetVRef() const = 0;

    static constexpr uint16_t MAX   = 0xFFF;
    static constexpr uint16_t MIN   = 0x000;
    static constexpr uint16_t RANGE = MAX - MIN;
};

#endif  // INCLUDE_ADC_INTERFACE_H_
