//
// Created by kifir on 6/18/24.
//

#include "pch.h"

#include "test_base.h"

void TestBase::SetUp() {
    eth = std::make_unique<EngineTestHelper>(engine_type_e::TEST_ENGINE);
}

void TestBase::TearDown() {
    eth.reset();
}

TestEngineConfiguration& TestBase::getTestEngineConfiguration() {
    return TestEngineConfiguration::getInstance();
}

TestPersistentConfiguration& TestBase::getTestPersistentConfiguration() {
    return TestPersistentConfiguration::getInstance();
}

TestEngineState& TestBase::getTestEngineState() {
    return TestEngineState::getInstance();
}

void TestBase::setUpEngineConfiguration(const EngineConfig& config) {
    // Launch Control
    getTestEngineConfiguration().configureLaunchActivatePin(config.getLaunchActivatePin());
    getTestEngineConfiguration().configureLaunchActivateInverted(config.getLaunchActivateInverted());
    getTestEngineConfiguration().configureLaunchControlEnabled(config.getLaunchControlEnabled());
    getTestEngineConfiguration().configureLaunchRpm(config.getLaunchRpm());
    getTestEngineConfiguration().configureLaunchRpmWindow(config.getLaunchRpmWindow());
    getTestEngineConfiguration().configureLaunchCorrectionsEndRpm(config.getLaunchCorrectionsEndRpm());
    getTestEngineConfiguration().configureIgnitionRetardEnable(config.getIgnitionRetardEnable());
    getTestEngineConfiguration().configureIgnitionRetard(config.getIgnitionRetard());
    getTestEngineConfiguration().configureSmoothRetardMode(config.getSmoothRetardMode());
    getTestEngineConfiguration().configureEnableIgnitionCut(config.getEnableIgnitionCut());
    getTestEngineConfiguration().configureInitialIgnitionCutPercent(config.getInitialIgnitionCut());
    getTestEngineConfiguration().configureFinalIgnitionCutPercentBeforeLaunch(config.getFinalIgnitionCutBeforeLaunch());

    // Shift Torque Reduction (Flat Shift)
    getTestEngineConfiguration().configureTorqueReductionEnabled(config.getTorqueReductionEnabled());
    getTestEngineConfiguration().configureTorqueReductionActivationMode(config.getTorqueReductionActivationMode());
    getTestEngineConfiguration().configureTorqueReductionTriggerPin(config.getTorqueReductionTriggerPin());
    getTestEngineConfiguration().configureTorqueReductionButtonInverted(config.getTorqueReductionTriggerPinInverted());
    getTestEngineConfiguration().configureLimitTorqueReductionTime(config.getLimitTorqueReductionTime());
    getTestEngineConfiguration().configureTorqueReductionTime(config.getTorqueReductionTime());
    getTestEngineConfiguration().configureTorqueReductionArmingRpm(config.getTorqueReductionArmingRpm());
    getTestEngineConfiguration().configureTorqueReductionArmingApp(config.getTorqueReductionArmingApp());
    getTestEngineConfiguration().configureTorqueReductionIgnitionCut(config.getTorqueReductionIgnitionCut());
    getTestEngineConfiguration().configureTorqueReductionIgnitionRetard(config.getTorqueReductionIgnitionRetard());

    // Injector
    getTestEngineConfiguration().configureInjectorFlowAsMassFlow(config.getInjectorFlowAsMassFlow());
    getTestEngineConfiguration().configureInjectorFlow(config.getInjectorFlow());
    getTestEngineConfiguration().configureInjectorBattLagCorr(config.getInjectorBattLagCorr());

    // Secondary injector
    getTestEngineConfiguration().configureInjectorSecondaryFlow(config.getInjectorSecondaryFlow());
    getTestEngineConfiguration().configureInjectorSecondaryBattLagCorr(config.getInjectorSecondaryBattLagCorr());

    // Staged injection
    getTestEngineConfiguration().configureEnableStagedInjection(config.getStagedInjectionEnabled());
}

void TestBase::periodicFastCallback() {
    // run the ignition math
    engine->periodicFastCallback();
}

void TestBase::periodicSlowCallback() {
    engine->periodicSlowCallback();
}

void TestBase::updateRpm(const float rpm) {
    Sensor::setMockValue(SensorType::Rpm, rpm);
    periodicFastCallback();
}

void TestBase::updateApp(const std::optional<float> app) {
    if (app.has_value()) {
        Sensor::setMockValue(SensorType::DriverThrottleIntent, app.value());
    } else {
        Sensor::resetMockValue(SensorType::DriverThrottleIntent);
    }
    periodicFastCallback();
}
