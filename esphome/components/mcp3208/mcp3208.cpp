#include "mcp3208.h"
#include "esphome/core/log.h"

namespace esphome {
namespace mcp3208 {

static const char *const TAG = "mcp3208";

float MCP3208::get_setup_priority() const { return setup_priority::HARDWARE; }

void MCP3208::setup() {
  ESP_LOGCONFIG(TAG, "Setting up mcp3208");
  this->spi_setup();
}

void MCP3208::dump_config() {
  ESP_LOGCONFIG(TAG, "MCP3208:");
  LOG_PIN("  CS Pin:", this->cs_);
}

float MCP3208::read_data(uint8_t pin) {
  uint8_t data_msb, data_lsb = 0;

//  uint8_t command = ((0x01 << 7) |          // start bit
//                     ((pin & 0x07) << 4));  // channel number

  uint8_t command1 = ((0x01 << 2) |          // start bit
                     ((pin & 0x04) >> 2));  // channel number

  uint8_t command2 = (pin & 0x03) << 6;

  this->enable();
//  this->transfer_byte(0x01);

  data_msb = this->transfer_byte(command1);
  data_lsb = this->transfer_byte(command2);

  this->disable();

  int data = data_msb << 8 | data_lsb;

  return data / 4095.0f;
}

MCP3208Sensor::MCP3208Sensor(MCP3208 *parent, const std::string &name, uint8_t pin, float reference_voltage)
    : PollingComponent(1000), parent_(parent), pin_(pin) {
  this->set_name(name);
  this->reference_voltage_ = reference_voltage;
}

float MCP3208Sensor::get_setup_priority() const { return setup_priority::DATA; }

void MCP3208Sensor::setup() { LOG_SENSOR("", "Setting up MCP3208 Sensor '%s'...", this); }
void MCP3208Sensor::dump_config() {
  ESP_LOGCONFIG(TAG, "MCP3208Sensor:");
  ESP_LOGCONFIG(TAG, "  Pin: %u", this->pin_);
  ESP_LOGCONFIG(TAG, "  Reference Voltage: %.2fV", this->reference_voltage_);
}
float MCP3208Sensor::sample() {
  float value_v = this->parent_->read_data(pin_);
  value_v = (value_v * this->reference_voltage_);
  return value_v;
}
void MCP3208Sensor::update() { this->publish_state(this->sample()); }

}  // namespace mcp3208
}  // namespace esphome