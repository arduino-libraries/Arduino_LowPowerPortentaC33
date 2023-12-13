#include "r_lpm.h"
#include "Arduino.h"
#include "FspTimer.h"

class LowPower {
    public:
        LowPower();
        void sleep();
        void deepSleep();
        void setWakeupPin(uint8_t pin);
        void setWakeupRTC();
        static void delay(long ms);
    private:
        int getStandbyWakeIRQChannelForPin(uint8_t pin);
        int getDeepSleepWakeIRQChannelForPin(uint8_t pin);
        uint16_t deepSleepWakeupSource;
        uint16_t standbyWakeupSource;
        lpm_instance_ctrl_t automatic_sleep_ctrl_block;
        lpm_cfg_t automatic_sleep_cfg;
        lpm_instance_ctrl_t lpm_voluntary_sleep_ctrl;
        lpm_cfg_t lpm_voluntary_sleep_cfg;
        FspTimer *  automaticSleepTimer;
};

