use glob::{glob, Paths};

// possible implementation of more dimenstions
enum ReaderToken {
    OVERWORLD,
}

pub struct TextureReader {
    tn_buffer: Vec<Paths>,
    sn_buffer: Vec<Paths>,
}

impl TextureReader {
    fn new() -> Self {
        Self {
            tn_buffer: Vec::new(),
            sn_buffer: Vec::new(),
        }
    }

    fn read_textures(&mut self, txtclass: ReaderToken) -> Vec<Paths> {
        let path: String =  {
            match txtclass {
                ReaderToken::OVERWORLD => String::from("../src-engine/resources/overworld"),
                _ => panic!("No such directory."),
            }
        };
        let directory = glob(&path).expect("Failed to read.");
        for entry in directory {
            match entry {
                Ok(path) => self.tn_buffer.push(path),
                Err(e) => println!("Error: {}", e),
            }
        }
        return self.tn_buffer;
    }
    fn read_shaders(&mut self, shader_class: ReaderToken) -> Vec<Paths> {
        todo!();
    }
}
