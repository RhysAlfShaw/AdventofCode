use std::fs::File;
use std::io::{self, BufRead};
use std::env;
use std::collections::HashSet;

fn print_advend_of_code_line() {
    println!(" Advent of Code 2024 - Day 10 ");
}

fn read_file_into_2d_array(file_path: &str) -> Result<Vec<Vec<i32>>, io::Error> {
    let file = File::open(file_path)?;
    let reader = io::BufReader::new(file);
    let mut digits_2d = Vec::new();

    for line in reader.lines() {
        let line = line?;
        let row: Result<Vec<i32>, _> = line
            .chars()
            .map(|c| c.to_digit(10))
            .collect::<Option<Vec<_>>>()
            .map(|digits| digits.into_iter().map(|d| d as i32).collect())
            .ok_or_else(|| io::Error::new(io::ErrorKind::InvalidData, "Invalid digit found in line"));

        match row {
            Ok(row) => digits_2d.push(row),
            Err(e) => {
                eprintln!("Error parsing line into digits: {}", e);
            }
        }
    }

    Ok(digits_2d)
}

fn get_neighbors(r: usize, c: usize, rows: usize, cols: usize) -> Vec<(usize, usize)> {
    let directions = [(-1, 0), (1, 0), (0, -1), (0, 1)];
    let mut neighbors = Vec::new();
    for (dr, dc) in directions.iter() {
        let new_r = r as i32 + dr;
        let new_c = c as i32 + dc;
        if new_r >= 0 && new_r < rows as i32 && new_c >= 0 && new_c < cols as i32 {
            neighbors.push((new_r as usize, new_c as usize));
        }
    }
    neighbors
}

fn count_distinct_trails(grid: &Vec<Vec<i32>>, start_r: usize, start_c: usize) -> i32 {
    let rows = grid.len();
    let cols = grid[0].len();
    let mut distinct_paths = HashSet::new();
    let mut visited = vec![vec![false; cols]; rows];

    fn dfs(
        grid: &Vec<Vec<i32>>, 
        r: usize, 
        c: usize, 
        visited: &mut Vec<Vec<bool>>, 
        path: &mut Vec<(usize, usize)>,
        distinct_paths: &mut HashSet<Vec<(usize, usize)>>
    ) {
        visited[r][c] = true;
        path.push((r, c));

        // check if we've reached a '9'
        if grid[r][c] == 9 {
            distinct_paths.insert(path.clone());
        }

        // explore neighbors
        let rows = grid.len();
        let cols = grid[0].len();
        for (new_r, new_c) in get_neighbors(r, c, rows, cols) {
            if !visited[new_r][new_c] && grid[new_r][new_c] == grid[r][c] + 1 {
                dfs(grid, new_r, new_c, visited, path, distinct_paths);
            }
        }
        visited[r][c] = false;
        path.pop();
    }

    let mut initial_path = Vec::new();
    dfs(grid, start_r, start_c, &mut visited, &mut initial_path, &mut distinct_paths);

    distinct_paths.len() as i32
}

fn count_reachable_9s(
    grid: &Vec<Vec<i32>>,
    r: usize,
    c: usize,
    visited: &mut Vec<Vec<bool>>,
) -> i32 {
    let rows = grid.len();
    let cols = grid[0].len();
    let mut count = 0;

    visited[r][c] = true;

    if grid[r][c] == 9 {
        count += 1;
    }

    for (new_r, new_c) in get_neighbors(r, c, rows, cols) {
        if !visited[new_r][new_c] && grid[new_r][new_c] == grid[r][c] + 1 {
            count += count_reachable_9s(grid, new_r, new_c, visited);
        }
    }

    count
}

fn calculate_trailhead_rating(grid: &Vec<Vec<i32>>, r: usize, c: usize) -> i32 {
    let rows = grid.len();
    let cols = grid[0].len();
    let mut total_trails = 0;

    for (new_r, new_c) in get_neighbors(r, c, rows, cols) {
        if grid[new_r][new_c] == grid[r][c] + 1 {
            total_trails += count_distinct_trails(grid, new_r, new_c);
        }
    }

    total_trails
}

fn search_trail_heads(grid: &Vec<Vec<i32>>) -> Vec<(usize, usize)> {
    let mut trail_heads = Vec::new();
    for r in 0..grid.len() {
        for c in 0..grid[0].len() {
            if grid[r][c] == 0 {
                trail_heads.push((r, c));
            }
        }
    }
    trail_heads
}

fn main() {

    print_advend_of_code_line();

    let cwd = env::current_dir().unwrap();

    let file_path_test = format!("{}/test_input.txt", cwd.display());

    println!("Reading file: {}", file_path_test);
    let integers = read_file_into_2d_array(&file_path_test).unwrap();
    // start a timer
    let start = std::time::Instant::now();
    // part 1: Count reachable 9s
    let trail_heads = search_trail_heads(&integers);
    let mut total = 0;
    for (r, c) in &trail_heads {
        let mut visited = vec![vec![false; integers[0].len()]; integers.len()];
        let count = count_reachable_9s(&integers, *r, *c, &mut visited);
        total += count;
        // println!("Trail head at ({}, {}), reachable 9s: {}", r, c, count);
    }
    println!("Test Part 1 -- Sum of Scores: {}", total);

    // part 2: Calculate trailhead ratings
    let mut total_rating = 0;
    for (r, c) in trail_heads {
        let rating = calculate_trailhead_rating(&integers, r, c);
        total_rating += rating;
        // println!("Trail head at ({}, {}), rating: {}", r, c, rating);
    }
    println!("Test Part 2 -- Total Rating: {}", total_rating);

    // now run on the real input
    let file_path_real = format!("{}/input.txt", cwd.display());
    println!("Reading file: {}", file_path_real);
    let integers_real = read_file_into_2d_array(&file_path_real).unwrap();

    let trail_heads_real = search_trail_heads(&integers_real);
    let mut total_real = 0;
    for (r, c) in &trail_heads_real {
        let mut visited = vec![vec![false; integers_real[0].len()]; integers_real.len()];
        let count = count_reachable_9s(&integers_real, *r, *c, &mut visited);
        total_real += count;
        // println!("Trail head at ({}, {}), reachable 9s: {}", r, c, count);
    }
    println!(" -------------------------------- ");
    println!("Real Part 1 -- Sum of Scores: {}", total_real);

    let mut total_rating_real = 0;
    for (r, c) in trail_heads_real {
        let rating = calculate_trailhead_rating(&integers_real, r, c);
        total_rating_real += rating;
        // println!("Trail head at ({}, {}), rating: {}", r, c, rating);
    }
    println!("Real Part 2 -- Total Rating: {}", total_rating_real);

    // stop the timer
    let duration = start.elapsed();
    println!("Time elapsed is: {:?}", duration);
    println!("Done!");
}
