use crate::{atmos, units::{Velocity, Length, Temperature, Pressure}};

mod grib;

#[test]
fn test_true_airspeed_1() {
    let online_tas = Velocity::from_knots(483.0);

    let h = Length::from_feet(35_000.0);
    let t = Temperature::from_celsius(-39.0);
    let p = atmos::calculate_pressure_at_alt(h, Length::default(), atmos::ISA_STD_PRES, t);
    let cas = Velocity::from_knots(260.0);
    let qc = atmos::calculate_impact_pressure_at_cas(cas);
    let m = atmos::calculate_mach_number(qc, p);
    let eas = atmos::calculate_eas(m, p);
    let atmos_tas = atmos::convert_mach_to_tas(m, t);
    let atmos_tas2 = atmos::convert_ias_to_tas(cas, p, h, h, t);

    println!("\tMach: \t\t{}", m);
    println!("\tEAS: \t\t{}kts", eas.as_knots());
    println!("\tOnline TAS: \t{}kts", online_tas.as_knots());
    println!("\tAtmos. TAS: \t{}kts", atmos_tas.as_knots());
    println!("\tAtmos. TAS: \t{}kts", atmos_tas2.0.as_knots());

    assert!((atmos_tas.as_knots() - online_tas.as_knots()).abs() <= 5.0);
}

#[test]
fn test_convert_tas_to_ias_1() {
    let (ias, _mach) = atmos::convert_tas_to_ias(
        Velocity::from_knots(421.0),
        Pressure::from_hectopascals(250.0),
        Length::from_feet(35_269.0),
        Length::from_feet(34_873.0),
        Temperature::from_kelvin(221.0),
    );

    assert!((ias.as_knots() - 250.0) <= 1.0);
}