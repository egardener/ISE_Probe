var i2c = new I2C();
i2c.setup({
  scl: D17,
  sda: D16,
  bitrate: 400000
});
var mv_obj = require("http://localhost:8000/ISE_Probe.js").connect(i2c, 0x4f);

function temp() {
  mv_obj.measureTemp(function(d) {
    console.log("tempC: " + d.tempC);
  });
}

function mv() {
  mv_obj.measuremV(function(d) {
    console.log("mV: " + d.mV);
  });
}

function config() {
  console.log("Config:");
  console.log("  offset: " + mv_obj.getCalibrateOffset());
  console.log("  dual point: " + mv_obj.usingDualPoint());
  console.log("  low reference / read: " + mv_obj.getCalibrateLowReference() + " / " + mv_obj.getCalibrateLowReading());
  console.log("  high reference / read: " + mv_obj.getCalibrateHighReference() + " / " + mv_obj.getCalibrateHighReading());
  console.log("  temp. compensation: : " + mv_obj.usingTemperatureCompensation());
  console.log("  version: " + mv_obj.getVersion().toString(16));
}

function cal(solutionmV) {
  if (solutionmV) {
    mv_obj.calibrateSingle(solutionmV, function(d) {
      console.log("  offset: " + mv_obj.getCalibrateOffset());
    });
  }
}

function low(solutionmV) {
  if (solutionmV) {
    mv_obj.calibrateProbeLow(solutionmV, function(d) {
      console.log("  low reference / read: " + mv_obj.getCalibrateLowReference() + " / " + mv_obj.getCalibrateLowReading());
    });
  }
  console.log("  low reference / read: " + mv_obj.getCalibrateLowReference() + " / " + mv_obj.getCalibrateLowReading());
}

function high(solutionmV) {
  if (solutionmV) {
    mv_obj.calibrateProbeHigh(solutionmV, function(d) {
      console.log("  high reference / read: " + mv_obj.getCalibrateHighReference() + " / " + mv_obj.getCalibrateHighReading());
    });
  }
  console.log("  high reference / read: " + mv_obj.getCalibrateHighReference() + " / " + mv_obj.getCalibrateHighReading());
}

save();