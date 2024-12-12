use std::collections::{HashSet, VecDeque};
use std::fs::File;
use std::io::{BufRead, BufReader};
use std::hash::{Hash, Hasher};
use std::time::Instant;

// custom type to make float coordinates hashable and comparable
#[derive(Clone, Copy, Debug)]
struct FloatCoord {
    x: f32,
    y: f32,
}
// implement the PartialEq trait for FloatCoord
impl PartialEq for FloatCoord {
    fn eq(&self, other: &Self) -> bool {
        // compare with a small epsilon for float comparison
        (self.x - other.x).abs() < 1e-6 && (self.y - other.y).abs() < 1e-6
    }
}

impl Eq for FloatCoord {}

impl Hash for FloatCoord {
    fn hash<H: Hasher>(&self, state: &mut H) {
        // Convert float to bits for hashing
        self.x.to_bits().hash(state);
        self.y.to_bits().hash(state);
    }
}


// read the input file and convert it to a 2D grid of characters
fn open_file(file_path: &str) -> Vec<Vec<char>> {
    let file = File::open(file_path).expect("Unable to open file");
    let reader = BufReader::new(file);
    reader
        .lines()
        .map(|line| line.expect("Could not read line").chars().collect())
        .collect()
}

// create regions of connected cells with the same character
fn create_regions(grid: &Vec<Vec<char>>) -> Vec<HashSet<(i32, i32)>> {
    let rows = grid.len() as i32;
    let cols = grid[0].len() as i32;
    let mut regions = Vec::new();
    let mut seen = HashSet::new();

    for row in 0..rows {
        for col in 0..cols {
            // skip if this cell has already been processed
            if seen.contains(&(row, col)) {
                continue;
            }

            let crop = grid[row as usize][col as usize];
            let mut region = HashSet::new();
            let mut queue = VecDeque::new();

            region.insert((row, col));
            seen.insert((row, col));
            queue.push_back((row, col));

            // flood fill algorithm to find connected cells
            while let Some((cr, cc)) = queue.pop_front() {
                // check adjacent cells (up, down, left, right)
                for (nr, nc) in [
                    (cr - 1, cc), // up
                    (cr + 1, cc), // down
                    (cr, cc - 1), // left
                    (cr, cc + 1), // right
                ] {
                    // boundary and type checks
                    if nr < 0 || nc < 0 || nr >= rows || nc >= cols {
                        continue;
                    }

                    if grid[nr as usize][nc as usize] != crop {
                        continue;
                    }

                    if region.contains(&(nr, nc)) {
                        continue;
                    }

                    region.insert((nr, nc));
                    seen.insert((nr, nc));
                    queue.push_back((nr, nc));
                }
            }

            regions.push(region);
        }
    }

    regions
}

// calculate the perimeter of a region
fn perimeter(region: &HashSet<(i32, i32)>) -> usize {
    let mut output = 0;
    for (row, col) in region {
        output += 4;
        for (nrow, ncol) in [
            (*row + 1, *col),
            (*row - 1, *col),
            (*row, *col - 1),
            (*row, *col + 1),
        ] {
            if region.contains(&(nrow, ncol)) {
                output -= 1;
            }
        }
    }
    output
}

// calculate the number of special sides (corners) in a region
fn sides(region: &HashSet<(i32, i32)>) -> usize {
    let mut corner_candidates = HashSet::new();
    
    // corner candidate points
    for (row, col) in region {
        corner_candidates.extend([
            FloatCoord { x: *row as f32 - 0.5, y: *col as f32 - 0.5 },
            FloatCoord { x: *row as f32 + 0.5, y: *col as f32 - 0.5 },
            FloatCoord { x: *row as f32 + 0.5, y: *col as f32 + 0.5 },
            FloatCoord { x: *row as f32 - 0.5, y: *col as f32 + 0.5 },
        ]);
    }

    let mut corners = 0;
    
    // corner configurations
    for candidate in corner_candidates {
        let config: Vec<bool> = [
            FloatCoord { x: candidate.x - 0.5, y: candidate.y - 0.5 },
            FloatCoord { x: candidate.x + 0.5, y: candidate.y - 0.5 },
            FloatCoord { x: candidate.x + 0.5, y: candidate.y + 0.5 },
            FloatCoord { x: candidate.x - 0.5, y: candidate.y + 0.5 },
        ]
        .iter()
        .map(|coord| {
            region.contains(&(coord.x as i32, coord.y as i32))
        })
        .collect();

        let number = config.iter().filter(|&&x| x).count();

        corners += match number {
            1 => 1,
            2 if config == [true, false, true, false] || 
                 config == [false, true, false, true] => 2,
            3 => 1,
            _ => 0
        };
    }

    corners
}

fn calculate_sum(regions: &Vec<HashSet<(i32, i32)>>, func: fn(&HashSet<(i32, i32)>) -> usize) -> usize {
    regions.iter().map(|region| region.len() * func(region)).sum()
}

fn process_grid(path: &str) {
    let grid = open_file(path);
    let regions = create_regions(&grid);
    
    let sum_1 = calculate_sum(&regions, perimeter);
    let sum_2 = calculate_sum(&regions, sides);
    
    println!("Part 1: {}", sum_1);
    println!("Part 2: {}", sum_2);
}


fn main() {
    println!("\n--- Advent of Code 2024 ---");
    let start = std::time::Instant::now();
    println!("TEST");
    let test_path = "test_input.txt";
    process_grid(test_path);

    println!("\nREAL");
    let real_path = "input.txt";
    process_grid(real_path);
    // print the time elapsed
    println!("\nCompleted in {}ms", start.elapsed().as_millis());
}   