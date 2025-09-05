use std::fs;
use itertools::Itertools;

/// calculate the safety factor for a given set of robot positions
fn calculate_safety_factor(robots: &[(i64, i64, i64, i64)], total_rows: i64, total_cols: i64) -> usize {
    let mut ans = 1;
    for rr in [0, total_rows / 2 + 1] {
        for cc in [0, total_cols / 2 + 1] {
            let count = robots.iter()
                .filter(|&&(r, c, _, _)|
                     r >= rr && r < rr + total_rows / 2 &&
                     c >= cc && c < cc + total_cols / 2
                )
                .count();
            ans *= count;
        }
    }
    ans
}

/// parse input file into a vector of robot coordinates and velocities
fn parse_input(filename: &str) -> Vec<(i64, i64, i64, i64)> {
    let input = fs::read_to_string(filename)
        .expect("Error reading file");
    
    input
        .split(|c: char| !c.is_ascii_digit() && c != '-')
        .filter(|s| !s.is_empty())
        .filter_map(|s| s.parse::<i64>().ok())
        .tuples()
        .collect()
}

/// move robots according to their velocities, wrapping around the grid
fn update_robots(robots: &mut Vec<(i64, i64, i64, i64)>, total_rows: i64, total_cols: i64) {
    for (r, c, dr, dc) in robots.iter_mut() {
        *r = (*r + *dr).rem_euclid(total_rows);
        *c = (*c + *dc).rem_euclid(total_cols);
    }
}

/// check if all robot positions are unique
fn are_robot_positions_unique(robots: &[(i64, i64, i64, i64)]) -> bool {
    robots.iter()
        .map(|&(r, c, _, _)| (r, c))
        .all_unique()
}

/// visualize the grid with robots
fn visualize_grid(robots: &[(i64, i64, i64, i64)], total_rows: usize, total_cols: usize) {
    let mut grid = vec![vec!['.'; total_cols]; total_rows];
    for &(r, c, _, _) in robots {
        grid[r as usize][c as usize] = '#';
    }
    
    // transposing the grid
    let grid: Vec<Vec<char>> = (0..total_cols)
        .map(|c| (0..total_rows).map(|r| grid[r][c]).collect())
        .collect();
    
    for row in grid {
        for &cell in &row {
            if cell == '#' {
                print!("\x1b[0;32m#\x1b[0m");
            } else {
                print!("{}", cell);
            }
        }
        println!();
    }
}

fn main() {
    let (test_total_rows, test_total_cols) = (7, 11);
    let (main_total_rows, main_total_cols) = (101, 103);

    let mut test_robots = parse_input("test_input.txt");
    let mut part_1_test = 0;

    for i in 1.. {
        update_robots(&mut test_robots, test_total_rows, test_total_cols);
        if i == 100 {
            part_1_test = calculate_safety_factor(&test_robots, test_total_rows, test_total_cols);
            break;
        }
    }

    let mut robots = parse_input("input.txt");
    let (mut part_1, mut part_2) = (0, 0);

    for i in 1.. {
        update_robots(&mut robots, main_total_rows, main_total_cols);
        if i == 100 {
            part_1 = calculate_safety_factor(&robots, main_total_rows, main_total_cols);
        }
        if are_robot_positions_unique(&robots) {
            visualize_grid(&robots, main_total_rows as usize, main_total_cols as usize);
            part_2 = i;
            break;
        }
    }
    print!("------------------------------------\n");
    println!("Test Part 1: {}", part_1_test);
    print!("------------------------------------\n");
    println!("Part 1: {}", part_1);
    println!("Part 2: {}", part_2);
    print!("------------------------------------\n");
    print!("Done!\n");
}   
