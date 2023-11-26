#include "pch.h"

#include "defaults.h"
#include "table_helper.h"
#include "mazda_miata_vvt.h"

static void setBosch02880155868() {
	// http://www.boschdealer.com/specsheets/0280155868cs.jpg
	engineConfiguration->injector.battLagCorrBins[0] = 6;
	engineConfiguration->injector.battLagCorr[0] = 3.371;

	engineConfiguration->injector.battLagCorrBins[1] = 8;
	engineConfiguration->injector.battLagCorr[1] = 1.974;

	engineConfiguration->injector.battLagCorrBins[2] = 10;
	engineConfiguration->injector.battLagCorr[2] = 1.383;

	engineConfiguration->injector.battLagCorrBins[3] = 11;
	engineConfiguration->injector.battLagCorr[3] = 1.194;

	engineConfiguration->injector.battLagCorrBins[4] = 12;
	engineConfiguration->injector.battLagCorr[4] = 1.04;

	engineConfiguration->injector.battLagCorrBins[5] = 13;
	engineConfiguration->injector.battLagCorr[5] = 0.914;

	engineConfiguration->injector.battLagCorrBins[6] = 14;
	engineConfiguration->injector.battLagCorr[6] = 0.797;

	engineConfiguration->injector.battLagCorrBins[7] = 15;
	engineConfiguration->injector.battLagCorr[7] = 0.726;
}

static void setDefaultWarmupFuelEnrichment() {
	static const float bins[] =
	{
		-40,
		-30,
		-20,
		-10,
		0,
		10,
		20,
		30,
		40,
		50,
		60,
		70,
		80,
		90,
		100,
		110
	};

	copyArray(config->cltFuelCorrBins, bins);

	static const float values[] =
	{
		1.50,
		1.50,
		1.42,
		1.36,
		1.28,
		1.19,
		1.12,
		1.10,
		1.06,
		1.06,
		1.03,
		1.01,
		1,
		1,
		1,
		1
	};

	copyArray(config->cltFuelCorr, values);
}

static void setDefaultVETable() {
	setRpmTableBin(config->veRpmBins);
	setTable(config->veTable, 80);

	setRpmTableBin(config->baroCorrRpmBins);
	setLinearCurve(config->baroCorrPressureBins, 75, 105, 1);

	// Default baro table is all 1.0, we can't recommend a reasonable default here
	setTable(config->baroCorrTable, 1);

	// Give default axes for cylinder trim tables
	copyArray(config->fuelTrimRpmBins, { 1000, 3000, 5000, 7000 });
	copyArray(config->fuelTrimLoadBins, { 20, 50, 80, 100 });

	// Default axes for VE blends
	for (size_t i = 0; i < efi::size(config->veBlends); i++) {
		auto& blend = config->veBlends[i];
		setLinearCurve(blend.loadBins, 0, 100, 10);
		setLinearCurve(blend.rpmBins, 0, 7000);

		setLinearCurve(blend.blendBins, 0, 100);
		setLinearCurve(blend.blendValues, 0, 100);
	}
}

static void setDefaultFuelCutParameters() {
	engineConfiguration->coastingFuelCutEnabled = false;
	engineConfiguration->coastingFuelCutRpmLow = 1300;
	engineConfiguration->coastingFuelCutRpmHigh = 1500;
	engineConfiguration->coastingFuelCutTps = 2;
	engineConfiguration->coastingFuelCutMap = 30;
	engineConfiguration->coastingFuelCutClt = 60;
}

static void setDefaultStftSettings() {
	auto& cfg = engineConfiguration->stft;

	// Default to disabled
	engineConfiguration->fuelClosedLoopCorrectionEnabled = false;

	// Default to proportional mode (for wideband sensors)
	engineConfiguration->stftIgnoreErrorMagnitude = false;

	// 60 second startup delay - some O2 sensors are slow to warm up.
	cfg.startupDelay = 60;

	// Only correct in [12.0, 17.0]
	cfg.minAfr = 12;
	cfg.maxAfr = 17;

	// Above 60 deg C
	cfg.minClt = 60;

	// 0.5% deadband
	cfg.deadband = 0.5f;

	// Sensible region defaults
	cfg.maxIdleRegionRpm = 1000;
	cfg.maxOverrunLoad = 35;
	cfg.minPowerLoad = 85;

	// Sensible cell defaults
	for (size_t i = 0; i < efi::size(cfg.cellCfgs); i++) {
		// 30 second time constant - nice and slow
		cfg.cellCfgs[i].timeConstant = 30;

		/// Allow +-5%
		cfg.cellCfgs[i].maxAdd = 5;
		cfg.cellCfgs[i].maxRemove = -5;
	}
}

static const uint8_t tpsTpsTable[TPS_TPS_ACCEL_TABLE][TPS_TPS_ACCEL_TABLE] = {
/* Generated by TS2C on Tue Apr 18 21:29:16 EDT 2017*/
{/* 0 0	*//* 0 0.0*/0,	/* 1 10.0*/0,	/* 2 20.0*/0,	/* 3 35.0*/0,	/* 4 50.0*/0,	/* 5 65.0*/0,	/* 6 80.0*/0,	/* 7 100.0*/0,	},
{/* 1 10	*//* 0 0.0*/10,	/* 1 10.0*/0,	/* 2 20.0*/0,	/* 3 35.0*/0,	/* 4 50.0*/0,	/* 5 65.0*/0,	/* 6 80.0*/0,	/* 7 100.0*/0,	},
{/* 2 20	*//* 0 0.0*/17,	/* 1 10.0*/10,	/* 2 20.0*/0,	/* 3 35.0*/0,	/* 4 50.0*/0,	/* 5 65.0*/0,	/* 6 80.0*/0,	/* 7 100.0*/0,	},
{/* 3 35	*//* 0 0.0*/23,	/* 1 10.0*/17,	/* 2 20.0*/10,	/* 3 35.0*/0,	/* 4 50.0*/0,	/* 5 65.0*/0,	/* 6 80.0*/0,	/* 7 100.0*/0,	},
{/* 4 50	*//* 0 0.0*/28,	/* 1 10.0*/23,	/* 2 20.0*/17,	/* 3 35.0*/10,	/* 4 50.0*/0,	/* 5 65.0*/0,	/* 6 80.0*/0,	/* 7 100.0*/0,	},
{/* 5 65	*//* 0 0.0*/32,	/* 1 10.0*/28,	/* 2 20.0*/23,	/* 3 35.0*/17,	/* 4 50.0*/10,	/* 5 65.0*/0,	/* 6 80.0*/0,	/* 7 100.0*/0,	},
{/* 6 80	*//* 0 0.0*/35,	/* 1 10.0*/32,	/* 2 20.0*/28,	/* 3 35.0*/23,	/* 4 50.0*/17,	/* 5 65.0*/10,	/* 6 80.0*/0,	/* 7 100.0*/0,	},
{/* 7 100	*//* 0 0.0*/37,	/* 1 10.0*/35,	/* 2 20.0*/32,	/* 3 35.0*/28,	/* 4 50.0*/23,	/* 5 65.0*/17,	/* 6 80.0*/10,	/* 7 100.0*/0,	},
};

static void setMazdaMiataNbTpsTps() {
	setLinearCurve(config->tpsTpsAccelFromRpmBins, 0, 100, 10);
	setLinearCurve(config->tpsTpsAccelToRpmBins, 0, 100, 10);
	copyTable(config->tpsTpsAccelTable, tpsTpsTable);
}

static void setDefaultLambdaTable() {
	static constexpr float mapBins[] = {
		30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 150, 175, 200, 225, 250
	};
	copyArray(config->lambdaLoadBins, mapBins);

	setRpmTableBin(config->lambdaRpmBins);

	static constexpr float rowValues[] = {
		1,		1,		1,		1,		// 30, 40, 50, 60 kpa
		1,		0.95,	0.92,	0.90,	// 70, 80, 90, 100 kpa
		0.89,	0.88,	0.86,	0.84,	// 110, 120, 130, 150 kpa
		0.8,	0.77,	0.75,	0.73,	// 175, 200, 225, 250 kpa
	};

	// Set each row to the corresponding value from rowValues
	for (size_t i = 0; i < efi::size(config->lambdaTable); i++) {
		for (size_t j = 0; j < efi::size(config->lambdaTable[i]); j++) {
			config->lambdaTable[i][j] = rowValues[i];
		}
	}
}

void setDefaultWallWetting() {
#if !EFI_UNIT_TEST
	// todo: this is a reasonable default for what kinds of engines exactly?
	engineConfiguration->wwaeTau = 0.3;
	engineConfiguration->wwaeBeta = 0.3;
#endif // EFI_UNIT_TEST

	// linear reasonable bins
	setLinearCurve(engineConfiguration->wwCltBins, -40, 100, 1);
	setLinearCurve(engineConfiguration->wwMapBins, 10, 80, 1);

	// These values are derived from the GM factory tune for a gen3 LS engine
	// Who knows if they're good for anything else, but at least they look nice?
	static constexpr float tauClt[] = {
		1.45, 1.30, 1.17, 1.05, 0.90, 0.82, 0.75, 0.70
	};
	copyArray(engineConfiguration->wwTauCltValues, tauClt);

	static constexpr float tauMap[] = {
		0.38, 0.55, 0.69, 0.86, 0.90, 0.95, 0.97, 1.00
	};
	copyArray(engineConfiguration->wwTauMapValues, tauMap);

	static constexpr float betaClt[] = {
		0.73, 0.66, 0.57, 0.46, 0.38, 0.31, 0.24, 0.19
	};
	copyArray(engineConfiguration->wwBetaCltValues, betaClt);

	static constexpr float betaMap[] = {
		0.21, 0.40, 0.60, 0.79, 0.85, 0.90, 0.95, 1.00
	};
	copyArray(engineConfiguration->wwBetaMapValues, betaMap);
}

static void setDefaultLambdaProtection() {
	engineConfiguration->lambdaProtectionEnable = false;

	engineConfiguration->lambdaProtectionMinLoad = 60;
	engineConfiguration->lambdaProtectionMinRpm = 2500;
	engineConfiguration->lambdaProtectionMinTps = 50;
	engineConfiguration->lambdaProtectionTimeout = 0.5f;

	engineConfiguration->lambdaProtectionRestoreLoad = 30;
	engineConfiguration->lambdaProtectionRestoreRpm = 2000;
	engineConfiguration->lambdaProtectionRestoreTps = 20;
}

static void setDefaultPriming() {
	// These defaults are reasonable for ~500cc cylinders
	static constexpr int8_t primeBins[]     = { -40, -20,   0,  20, 40, 60, 80, 100 };
	static constexpr uint16_t primeValues[] = { 755, 605, 265, 140, 75, 50, 45,  40 };

	copyArray(engineConfiguration->primeBins, primeBins);
	copyArray(engineConfiguration->primeValues, primeValues);
}

void setDefaultFuel() {
	// Base injection configuration
	engineConfiguration->isInjectionEnabled = true;
	engineConfiguration->injectionMode = IM_SEQUENTIAL;

	/**
	 * By the way http://users.erols.com/srweiss/tableifc.htm has a LOT of data
	 */
	engineConfiguration->injector.flow = 200;
	engineConfiguration->stoichRatioPrimary = STOICH_RATIO;

	// 9.0 = E100 pure ethanol
	engineConfiguration->stoichRatioSecondary = 9.0f;

	// Injector deadtime
	setBosch02880155868();

	// Tables
	setFuelTablesLoadBin(10, 160);
	setRpmTableBin(config->injPhaseRpmBins);

	setRpmTableBin(engineConfiguration->tpsTspCorrValuesBins);
	setLinearCurve(engineConfiguration->tpsTspCorrValues, 1, 1);

	setDefaultVETable();
	setDefaultLambdaTable();

	setRpmTableBin(config->mapEstimateRpmBins);
	setLinearCurve(config->mapEstimateTpsBins, 0, 100);
	setTable(config->mapEstimateTable, 60);

	// most of rusEFI installations are still port injected, for GDI see 'setGDIFueling'
	// -400 will close the injector just before TDC at the end of the exhaust stroke,
	// around the time the intake valve opens.
	setTable(config->injectionPhase, -400.0f);

	// Charge temperature estimation
	engineConfiguration->tChargeMinRpmMinTps = 0.25;
	engineConfiguration->tChargeMinRpmMaxTps = 0.25;
	engineConfiguration->tChargeMaxRpmMinTps = 0.25;
	engineConfiguration->tChargeMaxRpmMaxTps = 0.9;
	engineConfiguration->tChargeMode = TCHARGE_MODE_RPM_TPS;
	engineConfiguration->tChargeAirCoefMin = 0.098f;
	engineConfiguration->tChargeAirCoefMax = 0.902f;
	engineConfiguration->tChargeAirFlowMax = 153.6f;
	engineConfiguration->tChargeAirIncrLimit = 1.0f;
	engineConfiguration->tChargeAirDecrLimit = 12.5f;

	// CLT correction table
	setDefaultWarmupFuelEnrichment();

	// IAT correction table
	// TODO

	// Closed loop fuel correction
	setDefaultStftSettings();

	// Decel fuel cut
	setDefaultFuelCutParameters();

	engineConfiguration->tpsAccelEnrichmentThreshold = 40; // TPS % change, per engine cycle

	setDefaultWallWetting();

	// TPS/TPS AE curve
	setMazdaMiataNbTpsTps();

	// Some reasonable reference pressure that many vehicles use
	engineConfiguration->fuelReferencePressure = 300;

	// Lambda protection defaults
	setDefaultLambdaProtection();

	setDefaultPriming();
}
