use std::fs::File;
use std::io::{self, BufRead};
use std::env;
use std::collections::HashSet;
use std::thread::sleep;
use std::time::Duration;
use std::io::Write;

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


fn count_distinct_trails_print(grid: &Vec<Vec<i32>>, start_r: usize, start_c: usize) -> i32 {
    let rows = grid.len();
    let cols = grid[0].len();
    let mut distinct_paths = HashSet::new();
    let mut visited = vec![vec![false; cols]; rows];
    let mut trace = grid.clone();

    fn dfs(
        grid: &Vec<Vec<i32>>, 
        r: usize, 
        c: usize, 
        visited: &mut Vec<Vec<bool>>, 
        path: &mut Vec<(usize, usize)>,
        distinct_paths: &mut HashSet<Vec<(usize, usize)>>,
        trace: &mut Vec<Vec<i32>>
    ) {
        visited[r][c] = true;
        path.push((r, c));
        // trace[r][c] = -1; // Mark as visited
        // Print the current path with colored output
        // print_grid_with_path(grid, visited, path);

        // check if we've reached a '9'
        if grid[r][c] == 9 {
            distinct_paths.insert(path.clone());
            // reset the grid
            *trace = grid.clone();
        }

        // explore neighbors
        let rows = grid.len();
        let cols = grid[0].len();
        for (new_r, new_c) in get_neighbors(r, c, rows, cols) {
            if !visited[new_r][new_c] && grid[new_r][new_c] == grid[r][c] + 1 {
            // print!("{} {}", new_r, new_c);
            // replace the value with -1 
            trace[new_r][new_c] = grid[new_r][new_c]+10;
            print!("{}[2J", 27 as char);
            // when number of trails increase we reset the grid
            let number_of_trails = distinct_paths.len() as i32;
            print_grid(trace, number_of_trails);
            sleep(Duration::from_millis(20));  
            // trace[new_r][new_c] = grid[new_r][new_c]; // Restore the original value

            dfs(grid, new_r, new_c, visited, path, distinct_paths, trace);
            }
        }
        visited[r][c] = false;
        path.pop();
    }

    let mut initial_path = Vec::new();
    dfs(grid, start_r, start_c, &mut visited, &mut initial_path, &mut distinct_paths, &mut trace);

    distinct_paths.len() as i32
}

fn get_neighbors_print(r: usize, c: usize, rows: usize, cols: usize) -> Vec<(usize, usize)> {
    let mut neighbors = Vec::new();
    if r > 0 { neighbors.push((r - 1, c)); }
    if r < rows - 1 { neighbors.push((r + 1, c)); }
    if c > 0 { neighbors.push((r, c - 1)); }
    if c < cols - 1 { neighbors.push((r, c + 1)); }
    neighbors
}

fn print_grid_with_path(grid: &Vec<Vec<i32>>, visited: &Vec<Vec<bool>>, path: &Vec<(usize, usize)>) {
    for (r, row) in grid.iter().enumerate() {
        for (c, &cell) in row.iter().enumerate() {
            if path.contains(&(r, c)) {
                print!("\x1b[32m{}\x1b[0m ", cell); // Green for part of the path
            } else if visited[r][c] {
                print!("\x1b[33m{}\x1b[0m ", cell); // Yellow for visited but not part of the path
            } else {
                print!("{}\x1b[0m ", cell); // Default for unvisited
            }
        }
        println!();
    }
    println!();
}

// Count Disint trails with color print of the array at each step with colors of the neighbors being explored


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

fn calculate_trailhead_rating_print(grid: &Vec<Vec<i32>>, r: usize, c: usize) -> i32 {
    let rows = grid.len();
    let cols = grid[0].len();
    let mut total_trails = 0;

    for (new_r, new_c) in get_neighbors(r, c, rows, cols) {
        if grid[new_r][new_c] == grid[r][c] + 1 {
            total_trails += count_distinct_trails_print(grid, new_r, new_c);
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



fn print_grid(grid: &Vec<Vec<i32>>, number_of_trails: i32) {
    const RESET: &str = "\x1b[0m";
    const PASTEL_COLORS: [&str; 10] = [
        // 10 level from blue 0 to white
        "\x1b[48;5;33m", // blue
        "\x1b[48;5;39m",
        "\x1b[48;5;45m",
        "\x1b[48;5;51m",
        "\x1b[48;5;87m",
        "\x1b[48;5;123m",
        "\x1b[48;5;159m",
        "\x1b[48;5;195m",
        "\x1b[48;5;231m",
        "\x1b[48;5;255m", // white
    ];

    // let min_val = grid.iter().flatten().cloned().min().unwrap_or(0);
    // let max_val = grid.iter().flatten().cloned().max().unwrap_or(1);
    println!("Number of trails: {}", number_of_trails);
    for r in 0..grid.len() {
        for c in 0..grid[0].len() {
            let mut value = grid[r][c];
            let color_index = (value % PASTEL_COLORS.len() as i32).abs() as usize;
            // if value is -1 set color to red
            if value >= 10 {
                value -= 10;
                print!("\x1b[48;5;196m{:2} ", value);
            } 
            else {
                print!("{}{:3}{}", PASTEL_COLORS[color_index], value,RESET);
            }
        }
        println!();
    }
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

    print_grid(&integers,0);
    
    println!("Real Part 1 -- Sum of Scores: {}", total_real);

    let mut total_rating_real = 0;
    for (r, c) in trail_heads_real {
        let rating = calculate_trailhead_rating(&integers_real, r, c);
        total_rating_real += rating;
        // println!("Trail head at ({}, {}), rating: {}", r, c, rating);
    }
    println!("Real Part 2 -- Total Rating: {}", total_rating_real);

    // wait 10 seconds
    
    while start.elapsed() < Duration::from_secs(10) {
        // count down from 10
        // colors are: red, green, yellow, blue, magenta, cyan
        let colors_time = ["\x1b[31m", "\x1b[32m", "\x1b[33m", "\x1b[34m", "\x1b[35m", "\x1b[36m"];
        let remaining = 10 - start.elapsed().as_secs();
        let i = remaining as usize % colors_time.len();
        // Clear the line before printing
        print!("\r");
        print!("Time untill Fun! : {}{}", colors_time[i], remaining);
        io::stdout().flush().unwrap();
        sleep(Duration::from_secs(1));
    }

    // println!(" -------------------------------- ");

    print!("{}[2J", 27 as char);

    
    // count_distinct_trails_print
    let trail_heads_test = search_trail_heads(&integers);
    let mut total_test = 0;
    for (r, c) in &trail_heads_test {
        let mut visited = vec![vec![false; integers[0].len()]; integers.len()];
        let distinct_paths = count_distinct_trails_print(&integers, *r, *c);
        println!("Trail head at ({}, {}), distinct paths: {}", r, c, distinct_paths);
        total_test += distinct_paths;
    }
    println!("Distinct Trails: {}", total_test);



    // println!(" -------------------------------- ");

    // stop the timer
    println!("Done!");
    print_grid(&integers_real,0);

    while start.elapsed() < Duration::from_secs(5) {
        // count down from 10
        // colors are: red, green, yellow, blue, magenta, cyan
        let colors_time = ["\x1b[31m", "\x1b[32m", "\x1b[33m", "\x1b[34m", "\x1b[35m", "\x1b[36m"];
        let remaining = 10 - start.elapsed().as_secs();
        let i = remaining as usize % colors_time.len();
        // Clear the line before printing
        print!("\r");
        print!("Time untill EXTRA FUN! : {}{}", colors_time[i], remaining);
        io::stdout().flush().unwrap();
        sleep(Duration::from_secs(1));
    }

    // println!(" -------------------------------- ");


    let trail_heads_real = search_trail_heads(&integers_real);
    let mut total_real = 0;
    for (r, c) in &trail_heads_real {
        let mut visited = vec![vec![false; integers[0].len()]; integers_real.len()];
        let distinct_paths = count_distinct_trails_print(&integers_real, *r, *c);
        println!("Trail head at ({}, {}), distinct paths: {}", r, c, distinct_paths);
        total_test += distinct_paths;
    }
    println!("Distinct Trails: {}", total_real);


}
