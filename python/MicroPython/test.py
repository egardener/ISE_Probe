import unittest
import math
from phprobe import phprobe

ph = phprobe(-1, 5, 4)


class TestMethods(unittest.TestCase):

    def test_version(self):
        self.assertEqual(ph.getVersion(), 0x1b)

    def test_use_temp_comp_true(self):
        ph.useTemperatureCompensation(True)
        self.assertEqual(ph.usingTemperatureCompensation(), True)

    def test_use_temp_comp_false(self):
        ph.useTemperatureCompensation(False)
        self.assertEqual(ph.usingTemperatureCompensation(), False)

    def test_use_dual_point_true(self):
        ph.useDualPoint(True)
        self.assertEqual(ph.usingDualPoint(), True)

    def test_use_dual_point_false(self):
        ph.useDualPoint(False)
        self.assertEqual(ph.usingDualPoint(), False)

    def test_reference_low(self):
        ph.setDualPointCalibration(1.01, 0, 0, 0)
        self.assertEqual(ph.getCalibrateLow(), 1.01)

    def test_reference_high(self):
        ph.setDualPointCalibration(0, 1.01, 0, 0)
        self.assertEqual(ph.getCalibrateHigh(), 1.01)

    def test_reading_low(self):
        ph.setDualPointCalibration(0, 0, 1.01, 0)
        self.assertEqual(ph.getCalibrateLowReading(), 1.01)

    def test_reading_high(self):
        ph.setDualPointCalibration(0, 0, 0, 1.01)
        self.assertEqual(ph.getCalibrateHighReading(), 1.01)

    def test_reset(self):
        n = float('nan')
        ph.reset()
        self.assertTrue(math.isnan(ph.getCalibrateLow()))
        self.assertTrue(math.isnan(ph.getCalibrateHigh()))
        self.assertTrue(math.isnan(ph.getCalibrateLowReading()))
        self.assertTrue(math.isnan(ph.getCalibrateHighReading()))

    def test_temperature(self):
        self.assertNotEqual(ph.measureTemp(), -127)

    def test_calibrate_single():
        ph.calibrateSingle(7.0)
        self.assertAlmostEqual(first=ph.getCalibrateOffset(), second=7.0, delta=0.5)

    def test_measure_ph():
        self.assertAlmostEqual(first=ph.measurepH(), second=7.0, delta=0.5)


if __name__ == '__main__':
    unittest.main()
