use std::sync::{Arc, Mutex};
use std::thread;
use std::collections::HashMap;


fn print_advend_of_code_line() {
    println!(" Advent of Code 2024 - Day 10 ");
}

fn read_data_into_1d_vector(file_path: &str) -> Vec<i64> {
    // each item is seperate by a space
    let data = std::fs::read_to_string(file_path).expect("Error reading input file");
    let data: Vec<i64> = data
        .split_whitespace()
        .map(|x| x.parse().expect("Not a number"))
        .collect();
    return data;
}

fn blick(data: Vec<i64>, cache: &mut HashMap<i64, Vec<i64>>) -> Vec<i64> {
    let mut new_data = Vec::with_capacity(data.len() * 2); // Pre-allocate assuming worst case
    
    for &num in &data {
        if let Some(cached_result) = cache.get(&num) {
            new_data.extend(cached_result.clone());
        } else {
            let mut result = Vec::new();
            if num == 0 {
                result.push(1);
            } else if num.to_string().len() % 2 == 0 {
                let num_str = num.to_string();
                let half = num_str.len() / 2;
                let (first_half, second_half) = num_str.split_at(half);
                result.push(first_half.parse().unwrap());
                result.push(second_half.parse().unwrap());
            } else {
                result.push(num * 2024);
            }
            cache.insert(num, result.clone());
            new_data.extend(result);
        }
    }

    new_data
}

fn main() {
    print_advend_of_code_line();
    let input_test_path = "test_input.txt";
    let input_path_real = "input.txt";

    println!("-------TEST-------");

    let mut cache = HashMap::new();
    let test_data = read_data_into_1d_vector(input_test_path);
    println!("Test data: {:?}", test_data);
    // loop 5 times to blick the data
    let mut test_data_blicked = test_data.clone();
    let num_blicks = 6;
    for _ in 0..num_blicks {
        test_data_blicked = blick(test_data_blicked, &mut cache);
        println!("Test data blicked: {:?}", test_data_blicked);
    }
    let length_test_data = test_data_blicked.len();
    println!("Sum of the test data: {}", length_test_data);

    println!("-------REAL-------");
    let real_data = read_data_into_1d_vector(input_path_real);
    // 
    println!("Real data: {:?}", real_data);
    // loop 25
    let mut real_data_blicked = real_data.clone();
    let num_blicks = 75;

    for i in 0..num_blicks {
        println!("Blick Number: {}", i);
        real_data_blicked = blick(real_data_blicked, &mut cache);
    }
    let length_real_data = real_data_blicked.len();

    println!("Sum of the real data: {}", length_real_data);
    
    // let real_data_blicked = Arc::new(real_data_blicked);
    // let length_real_data = Arc::new(Mutex::new(0));

    // let mut handles = vec![];

    // for i in 0..real_data_blicked.len() {
    //     let real_data_blicked = Arc::clone(&real_data_blicked);
    //     let length_real_data = Arc::clone(&length_real_data);

    //     let handle = thread::spawn(move || {
    //         let mut seed_data = vec![real_data_blicked[i]];
    //         // println!("Seed data: {:?}", seed_data);

    //         for _ in 0..num_blicks {
    //             println!("Thread: {}. Blick Number: {}",, _);
    //             seed_data = blick(seed_data);
    //             // println!("Seed data blicked: {:?}", seed_data);
    //         }

    //         let mut length_real_data = length_real_data.lock().unwrap();
    //         *length_real_data += seed_data.len();
    //     });

    //     handles.push(handle);
    // }

    // for handle in handles {
    //     handle.join().unwrap();
    // }

    // let length_real_data = *length_real_data.lock().unwrap();
    
    // println!("Sum of the real data: {}", length_real_data);
    
    for i in 0..num_blicks {
        println!("Blick Number: {}", i);
    
        real_data_blicked = blick(real_data_blicked, &mut cache);
        // println!("Real data blicked: {:?}", real_data_blicked);
    }
    let length_real_data = real_data_blicked.len();
    println!("Sum of the real data: {}", length_real_data);

}
