#include "pch.h"
#include "defaults.h"
#include "board_overrides.h"

// --- Helpery lokalne, static = tylko w tym pliku ---
static void balefi_setInjectorPins() {
    engineConfiguration->injectionPins[0] = Gpio::D12; 
    engineConfiguration->injectionPins[1] = Gpio::D13; 
    engineConfiguration->injectionPins[2] = Gpio::D11; 
    engineConfiguration->injectionPins[3] = Gpio::D10; 
}

static void balefi_setIgnitionPins() {
    engineConfiguration->ignitionPins[0] = Gpio::A8; 
    engineConfiguration->ignitionPins[1] = Gpio::A9; 
}

static void balefi_setupVbatt() {
    engineConfiguration->analogInputDividerCoefficient = 2.0f;
    engineConfiguration->vbattDividerCoeff = ((33.0 + 4.7) / 4.7)*2;
    engineConfiguration->vbattAdcChannel = EFI_ADC_4;
    engineConfiguration->adcVcc = 3.3f;
}

static void balefi_setSdCardSpi1NoCS() {
    engineConfiguration->isSdCardEnabled = true;
    engineConfiguration->sdCardSpiDevice = SPI_DEVICE_1;

    engineConfiguration->spi1mosiPin = Gpio::B5;
    engineConfiguration->spi1misoPin = Gpio::B4;
    engineConfiguration->spi1sckPin = Gpio::B3;
    engineConfiguration->is_enabled_spi_1 = true;
}

static void balefi_setSdCardSpi1() {
    balefi_setSdCardSpi1NoCS();
    engineConfiguration->sdCardCsPin = Gpio::B6;
}

static void balefi_setEtbConfig() {
    engineConfiguration->etbIo[0].controlPin = Gpio::D15; 
    engineConfiguration->etbIo[0].directionPin1 = Gpio::C6; 
    engineConfiguration->etbIo[0].disablePin = Gpio::D14; 
    engineConfiguration->etb_use_two_wires = false;
}

// --- Funkcje globalne dla innych plików (unikalne nazwy) ---
void balefi_setBoardConfigOverrides() {
    balefi_setupVbatt();
    balefi_setSdCardSpi1();
    //engineConfiguration->clt.config.bias_resistor = 4700;
    //engineConfiguration->iat.config.bias_resistor = 4700;
}

void balefi_setBoardDefaultConfiguration() {
    balefi_setInjectorPins();
    balefi_setIgnitionPins();
    balefi_setEtbConfig();

    engineConfiguration->tps1_1AdcChannel = EFI_ADC_13;
    engineConfiguration->tps1_2AdcChannel = EFI_ADC_12;
    engineConfiguration->map.sensor.hwChannel = EFI_ADC_3;
    engineConfiguration->ignitionKeyDigitalPin = Gpio::E15;

    setPPSInputs(EFI_ADC_1, EFI_ADC_0);
    engineConfiguration->enableAemXSeries = true;

    engineConfiguration->clt.adcChannel = EFI_ADC_11;
    engineConfiguration->iat.adcChannel = EFI_ADC_10;
    engineConfiguration->triggerInputPins[0] = Gpio::E4; 
    engineConfiguration->camInputs[0] = Gpio::E5; 
    engineConfiguration->vehicleSpeedSensorInputPin = Gpio::E6; 

    engineConfiguration->lps25BaroSensorScl = Gpio::B10;
    engineConfiguration->lps25BaroSensorSda = Gpio::B11;

    engineConfiguration->is_enabled_spi_3 = true;
    engineConfiguration->spi3misoPin = Gpio::C11;
    engineConfiguration->spi3sckPin = Gpio::C10;

    engineConfiguration->displayLogicLevelsInEngineSniffer = true;
    engineConfiguration->globalTriggerAngleOffset = 0;
    engineConfiguration->enableSoftwareKnock = true;

    engineConfiguration->canTxPin = Gpio::D1; 
    engineConfiguration->canRxPin = Gpio::D0; 
    engineConfiguration->mainRelayPin = Gpio::B15; 
    engineConfiguration->fanPin = Gpio::B13; 
    engineConfiguration->fuelPumpPin = Gpio::D8; 
    engineConfiguration->malfunctionIndicatorPin = Gpio::D9; 
    engineConfiguration->tachOutputPin = Gpio::B14;

    engineConfiguration->enableVerboseCanTx = true;
    engineConfiguration->etbFunctions[0] = DC_Throttle1;
    engineConfiguration->injectorCompensationMode = ICM_FixedRailPressure;

    //setCommonNTCSensor(&engineConfiguration->clt, 4700);
    //setCommonNTCSensor(&engineConfiguration->iat, 4700);
    setTPS1Calibration(100, 650);

    //setCrankOperationMode();
}

// --- Brakująca funkcja dla linkera (unikalna nazwa) ---
void setup_custom_board_overrides() {
    custom_board_DefaultConfiguration = balefi_setBoardConfigOverrides;
    custom_board_ConfigOverrides = balefi_setBoardDefaultConfiguration;
}

// --- Pozostałe funkcje lokalne ---
static Gpio OUTPUTS[] = {
    Gpio::D8, Gpio::D9, Gpio::D10, Gpio::D11, Gpio::D12, Gpio::D13,
    Gpio::B15, Gpio::B14, Gpio::B13, Gpio::A8, Gpio::A9
};

int getBoardMetaOutputsCount() {
    return efi::size(OUTPUTS);
}

int getBoardMetaLowSideOutputsCount() {
    return getBoardMetaOutputsCount() - 6;
}

Gpio* getBoardMetaOutputs() {
    return OUTPUTS;
}

Gpio getCommsLedPin() { return Gpio::A7; }
Gpio getRunningLedPin() { return Gpio::A6; }
Gpio getWarningLedPin() { return Gpio::A5; }
