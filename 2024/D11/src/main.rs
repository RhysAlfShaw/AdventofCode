use hashbrown::HashMap;


fn print_advend_of_code_line() {
    println!(" Advent of Code 2024 - Day 10 ");
}


fn read_in_data_to_string(file_path: &str) -> String {
    let file = std::fs::read_to_string(file_path).unwrap();
    file
}


fn blink(old_data: &HashMap<i64, usize>) -> HashMap<i64, usize> {
    // create new hashmap with the same capacity as the old hashmap
    // optimisation.
    let mut stones = HashMap::with_capacity(old_data.len());
    
    // iterate over the old hashmap
    for (&stone, &value) in old_data {
        match stone {
            // if the stone is 0, add 1 to the new hashmap
            0 => *stones.entry(1).or_default() += value,   
            _ => {
                // get the number of digits in the stone though the log base 10
                let digits = stone.ilog10() + 1; 
                // println!("{} {}", stone, digits);
                if digits % 2 == 0 {
                    // if the number of digits is even, split the stone in half
                    let half_digits = digits / 2;
                    let divisor = 10i64.pow(half_digits);
                    // store the key and the value in the new hashmap
                    *stones.entry(stone % divisor).or_default() += value; 
                    *stones.entry(stone / divisor).or_default() += value; 
                    // println!("{} {}", stone % divisor, stone / divisor);

                } else {
                    // if the number of digits is odd, multiply the stone by 2024
                    *stones.entry(stone * 2024).or_default() += value; 
                }
            }
        }
    }
    stones
}


fn main(){
    print_advend_of_code_line();
    let input_test = read_in_data_to_string("test_input.txt");
    
    let input_real = read_in_data_to_string("input.txt");
    // store stones as a hashmap with the stone as the key and the number of stones as the value
    let mut data_real = input_real.split(' ')
        .map(|w| (w.parse().unwrap(), 1))
        .collect::<HashMap<_, _>>(); 
    let mut data_test = input_test.split(' ')
        .map(|w| (w.parse().unwrap(), 1))
        .collect::<HashMap<_, _>>();
    // print the hashmap
    println!("{:?}", data_real);
    // start timer
    let start = std::time::Instant::now();
    let mut part_1 = 0;
    let mut part_1_test = 0;
    let number_of_blinks_part1 = 25;
    let number_of_blinks_part2 = 75;

    for i in 0..number_of_blinks_part2 {
        if i == number_of_blinks_part1 {
            part_1_test = data_test.values().sum();
        }
        data_test = blink(&data_test);
    }
    println!("Test data: {:?}", data_test);
    println!("Part 1:{} Part 2: {}", part_1_test, data_test.values().sum::<usize>());
    println!("---------------------------------");
    for i in 0..number_of_blinks_part2 {
        if i == number_of_blinks_part1 {
            part_1 = data_real.values().sum();
        }
        data_real = blink(&data_real);
        println!("{:?}", data_real)
    }

    println!("Part 1:{} Part 2: {}", part_1, data_real.values().sum::<usize>());
    // print the time taken
    println!("Time taken: {:?}", start.elapsed());
}