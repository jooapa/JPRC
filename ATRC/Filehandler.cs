namespace ATRC
{
    public partial class Filehandler
    {
        private static bool _was_last_re_dash = false;
        private static bool _was_last_private_variable = false;
        private static bool _was_last_variable = false;
        private static bool _next_value_symbol = false;
        /// <summary>
        /// Parse the contents of the file
        /// </summary>
        /// <param name="p_contents">File contents</param>
        /// <returns>block data and variable data</returns>
        private static (ATRCBlock[], ATRCVariable[]) ParseFile(string p_contents) {
            List<ATRCBlock> _blocks = new List<ATRCBlock>();
            List<ATRCVariable> _variables = new List<ATRCVariable>();
            List<ATRCVariable> _private_variable = new List<ATRCVariable>();

            string _current_word = "";
            string _whole_word = "";
            int _current_word_index = 0;
            int _start_looking_for_value = 0;
            bool looping = true;

            _was_last_private_variable = false;
            _was_last_variable = false;
            _next_value_symbol = false;
            _was_last_re_dash = false;
            
            for(int i = 0; i < p_contents.Length; i++){
                _current_word += p_contents[i];

                string _trim_current_word = _current_word.Trim();
                if(_trim_current_word == "") continue;

                // Check for variables:
                if(_trim_current_word.StartsWith('%')  || _trim_current_word.StartsWith("<%")){
                    if(_trim_current_word.StartsWith('%')){
                        // Public variable
                        _whole_word = _trim_current_word.Substring(1);
                        _was_last_variable = false;
                    } else {
                        // Private variable
                        _whole_word = _trim_current_word.Substring(2);
                        _was_last_private_variable = true;
                    }
                    // We have variable name
                    if(_trim_current_word.EndsWith('%')){
                        _next_value_symbol = true;
                        _start_looking_for_value = i;
                    }

                    if(_next_value_symbol){
                        _next_value_symbol = false;
                        _was_last_variable = true;
                    }

                    if(_trim_current_word.EndsWith('#')){
                        ATRCVariable __VARIABLE = new(){
                            Name = _whole_word[.._start_looking_for_value],
                            Value = _whole_word[_start_looking_for_value..^1],
                        };

                        if(_was_last_private_variable)
                            _private_variable.Add(__VARIABLE);
                        else
                            _variables.Add(__VARIABLE);
                    }

                } // Check for blocks 
                // else if(2==1){}
            }

            return (_blocks.ToArray(), _variables.ToArray());
        }

        /// <summary>
        /// Create a new atrc file. 
        /// p_mode: 
        /// </summary>
        /// <param name="p_filename">File to be created</param>
        /// <param name="p_mode">
        /// "c" for create - Will not overwrite existing file; Default value
        /// "cf" for create force - Will overwriste existing file
        /// </param>
        /// <param name="p_fileData">
        /// ATRCFileData object to be written to file, optional; null by default
        /// </param>
        /// <returns></returns>
        /// <exception cref="System.IO.IOException"></exception>
        public static ATRCFileData Create(string p_filename, string p_mode = "c", ATRCFileData? p_fileData = null) {
            ArgumentNullException.ThrowIfNull(p_filename);
            if(System.IO.Path.GetExtension(p_filename) != ".atrc") throw new System.IO.IOException("File is not an ATRC file");

            ATRCFileData _fileData = new()
            {
                Filename = p_filename,
            };

            System.IO.File.WriteAllText(p_filename, "");
            Debug.DebugConsole(_fileData.Filename, "created");
            // TODO PARSE FILE DATA
            return _fileData;
        }

        /// <summary>
        /// Open a file, set filepath first using Filename property
        /// </summary>
        /// <returns>
        /// ATRCFileData object
        /// </returns>
        public static ATRCFileData Read(string p_filename) {
            ArgumentNullException.ThrowIfNull(p_filename);
            if(System.IO.Path.GetExtension(p_filename) != ".atrc") throw new System.IO.IOException("File is not an ATRC file");
            
            ATRCFileData _fileData = new()
            {
                Filename = p_filename,
            };

            string _Contents = System.IO.File.ReadAllText(p_filename);
            (_fileData.Blocks, _fileData.Variables) = ParseFile(_Contents);

            Debug.DebugConsole(_fileData.Variables, "opened");
            
            return _fileData;
        }
    }
}