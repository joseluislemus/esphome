#pragma once

#include "esphome/core/component.h"
#include "esphome/core/esphal.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/spi/spi.h"
#include "esphome/components/voltage_sampler/voltage_sampler.h"

namespace esphome {
namespace mcp3208 {

class MCP3208 : public Component,
                public spi::SPIDevice<spi::BIT_ORDER_MSB_FIRST, spi::CLOCK_POLARITY_HIGH, spi::CLOCK_PHASE_LEADING,
                                      spi::DATA_RATE_1MHZ> {
 public:
  MCP3208() = default;

  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override;
  float read_data(uint8_t pin);

 protected:
};

class MCP3208Sensor : public PollingComponent, public sensor::Sensor, public voltage_sampler::VoltageSampler {
 public:
  MCP3208Sensor(MCP3208 *parent, const std::string &name, uint8_t pin, float reference_voltage);

  void set_reference_voltage(float reference_voltage) { reference_voltage_ = reference_voltage; }
  void setup() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override;
  float sample() override;

 protected:
  MCP3208 *parent_;
  uint8_t pin_;
  float reference_voltage_;
};

}  // namespace mcp3208
}  // namespace esphome