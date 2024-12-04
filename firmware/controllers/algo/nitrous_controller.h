//
// Created by kifir on 11/25/24.
//

#pragma once

#include "nitrous_control_state_generated.h"

class NitrousController : public nitrous_control_state_s, public EngineModule {
public:
    using interface_t = NitrousController;

    void onSlowCallback() override;
private:
    void updateArmingState();
    void updateTpsConditionSatisfied();
    void updateCltConditionSatisfied();
    void updateMapConditionSatisfied();
    void updateAfrConditionSatisfied();
    void updateRpmConditionSatisfied();

    bool checkTriggerPinState() const;
    bool checkLuaGauge() const;

    SensorType getLuaGauge() const;
};
