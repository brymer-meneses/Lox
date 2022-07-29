#[derive(PartialEq, Debug, Clone)]
pub struct SourceLocation {
    // start and end position of lines
    pub line_start: usize,
    pub line_end: usize,

    pub start: usize,
    pub end: usize,
}

impl SourceLocation {
    pub fn new_multiple_line(line_start: usize, line_end: usize, start: usize, end: usize) -> Self {
        return SourceLocation {
            line_start,
            line_end,
            start,
            end,
        };
    }

    pub fn new_single_line(line: usize, start: usize, end: usize) -> Self {
        return SourceLocation {
            line_end: line,
            line_start: line,
            start,
            end,
        };
    }
}
