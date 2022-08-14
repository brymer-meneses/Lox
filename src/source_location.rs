use std::cmp::{max, min};
use std::ops;

#[derive(PartialEq, Eq, Debug, Copy, Clone, Default)]
pub struct SourceLocation {
    // start and end position of lines
    pub line_start: usize,
    pub line_end: usize,

    pub start: usize,
    pub end: usize,
}

impl SourceLocation {
    pub fn new_multiple_line(line_start: usize, line_end: usize, start: usize, end: usize) -> Self {
        SourceLocation {
            line_start,
            line_end,
            start,
            end,
        }
    }

    pub fn new_single_line(line: usize, start: usize, end: usize) -> Self {
        SourceLocation {
            line_end: line,
            line_start: line,
            start,
            end,
        }
    }
    pub fn dummy() -> Self {
        SourceLocation {
            line_end: 0,
            line_start: 0,
            start: 0,
            end: 0,
        }
    }
}

impl ops::Add<SourceLocation> for SourceLocation {
    type Output = SourceLocation;

    fn add(self, rhs: SourceLocation) -> SourceLocation {
        SourceLocation {
            line_start: min(self.line_start, rhs.line_start),
            line_end: max(self.line_end, rhs.line_end),
            start: min(self.start, rhs.start),
            end: max(self.end, rhs.end),
        }
    }
}
