use itertools::Itertools;

fn read_txt(path: &str) -> String {
    std::fs::read_to_string(path).expect("Error reading file")
}

fn solve_equations(x1: i64, x2: i64, y1: i64, y2: i64, z1: i64, z2: i64) -> i64 {
    // the set of equations are:
    // x1 * a + y1 * b = z1
    // x2 * a + y2 * b = z2
    // these are number to be pressed, the cost is then a costs 3 and b costs 1
    // calculate the value of b using the given equations
    let numerator_b = z2 * x1 - z1 * x2;
    let denominator_b = y2 * x1 - y1 * x2;
    let b = numerator_b / denominator_b;

    // calculate the value of a using the value of b
    let numerator_a = z1 - b * y1;
    let a = numerator_a / x1;

    // verify the solution by plugging a and b back into the original equations
    let equation1_result = x1 * a + y1 * b;
    let equation2_result = x2 * a + y2 * b;

    // not a valid solution
    if (equation1_result, equation2_result) != (z1, z2) {
        return 0;
    }

    a * 3 + b 
}

fn main() {
    println!("\n--- Advent of Code 2024 ---");
    let start = std::time::Instant::now();

    let test_path = "test_input.txt";
    let test_input = read_txt(test_path);

    let path = "input.txt";
    let input = read_txt(path);
    // println!("{}", input);   
    let mut part1_sol_test = 0; 
    let mut part2_sol_test = 0;

    let mut part1_sol = 0;
    let mut part2_sol = 0;

    for l in test_input.split("\n\n") {
    
        // get the aruments for the function from the awful input.
        let (x1, x2, y1, y2, z1, z2) = l
            .split(|c: char| !c.is_ascii_digit())
            .filter(|w| !w.is_empty())
            .map(|w| w.parse().unwrap())
            .collect_tuple()
            .unwrap();

        part1_sol_test += solve_equations(x1, x2, y1, y2, z1, z2);
        part2_sol_test += solve_equations(x1, x2, y1, y2, z1 + 10000000000000, z2 + 10000000000000);
    }

    println!("Part 1 Test: {}", part1_sol_test);
    println!("Part 2 Test: {}", part2_sol_test);
    for l in input.split("\n\n") { // for each chunk of input
    
        // get the aruments for the function from the awful input.
        let (x1, x2, y1, y2, z1, z2) = l
            .split(|c: char| !c.is_ascii_digit()) // split by non digit characters
            .filter(|w| !w.is_empty()) // remove empty strings
            .map(|w| w.parse().unwrap()) // parse the strings into integers
            .collect_tuple() // collect the integers into a tuple
            .unwrap(); // unwrap the tuple

        part1_sol += solve_equations(x1, x2, y1, y2, z1, z2);
        part2_sol += solve_equations(x1, x2, y1, y2, z1 + 10000000000000, z2 + 10000000000000);
    }

    println!("Part 1: {}", part1_sol);
    println!("Part 2: {}", part2_sol);
    // time to run the program
    println!("\nCompleted in {}ms", start.elapsed().as_millis());
    println!("Done")
}