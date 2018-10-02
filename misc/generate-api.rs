// file is under construction

use std::fs::File;
use std::io::prelude::*;

const EXPORT_PREFIX: &'static str = "M2O_SERVER_API ";

const TARGETS: &'static [&'static str] = &[
    "../projects/shared/m2o_config.h",
    "../projects/server/api/general.h",
    "../projects/server/api/vehicle.h",
    "../projects/server/api/player.h",
    "../projects/server/api/pedestrian.h",
];

fn read_file(filename: &str) -> String {
    let mut file = match File::open(filename) {
        Err(err) => panic!("Couldn't open: {}", err),
        Ok(file) => file,
    };

    let mut data = String::new();
    match file.read_to_string(&mut data) {
        Err(err) => panic!("Couldn't read: {}", err),
        Ok(_) => (),
    };

    return data;
}

fn main() -> std::io::Result<()> {
    /* parse the `exportPrefix` file lines */
    let results: Vec<Vec<String>> = TARGETS
        .into_iter()
        .map(|file| read_file(file))
        .map(|data| data
            .split("\n")
            .filter(|substr| substr.find(EXPORT_PREFIX) != None)
            .map(|line| line
                .replace("{", "")
                .replace(EXPORT_PREFIX, "")
                .trim()
                .to_string()
            )
            .collect()
        )
        .collect()
    ;

    let matches: Vec<Vec<String>> = results
        .into_iter()
        .flatten()
        .collect::<Vec<String>>()
        .into_iter()
        .map(|line| line
            .replace("*m2o_", "* m2o_")
            .splitn(2, " m2o_")
            .map(|slice| slice.to_string())
            .collect()
        )
        .collect()
    ;

    // let vtableSignatures: Vec<String> = String::new(); {
    //     vtableSignatures.push(matches
    //         // .into_iter()
    //         .map(|pair|
    //             pair.
    //         )
    //     );
    // }

    for val in matches {
        println!("{:?}", val);
    }

    Ok(())
}
