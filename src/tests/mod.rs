use serde::{Deserialize, Serialize};

use crate::units::Angle;

mod geo;
mod aviation;
mod atmos;
mod magnetic;

#[test]
fn test_deserialize_angle_1(){
    
    #[derive(Default, Serialize, Deserialize, Debug)]
    struct Test {a: Angle, b: Angle}
    let json_str = r#"{"a": 0, "b": 34.353}"#;

    let result = serde_json::from_str::<Test>(&json_str);
    println!("{:?}", result);
}