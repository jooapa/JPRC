using System.Linq.Expressions;
using System.Reflection.Metadata;

namespace ATRC
{
    public partial class Filehandler
    {
        public partial class ATRCFileData{
            private bool _was_last_re_dash = false;
            private bool _was_last_private_variable = false;
            private bool _was_last_variable = false;
            private bool _next_value_symbol = false;
            /// <summary>
            /// Parse the value of a variable or key
            /// </summary>
            /// <param name="_line">Line containing value, non-trimmed</param>
            /// <param name="_line_number">Line number</param>
            /// <returns>Value, </returns>
            public object ParserValue(string _line, int _line_number){
                _line = _line.Trim();
                int _start_looking_for_value = _line.IndexOf('='); // Find the first = sign, this is where the value starts
                if(_start_looking_for_value == -1) throw new System.IO.IOException("Invalid variable or key value declaration at line " + _line_number);
                string _possible_value = _line.Substring(_start_looking_for_value + 1).Trim();
                string _final_value = ""; // This is the final value that will be returned
                bool _looking_for_variable = false;
                bool _looking_for_list = false;
                List<string> _list_values = new();

                string _variable_name = "";
                // We have to loop through the string to find the value, in case it is a string, list or contains ! in some point without \ before it
                foreach(char c in _possible_value){
                    if(c == '\\' && _was_last_re_dash){
                        _final_value += c;
                        _was_last_re_dash = false;
                        continue;
                    }
                    if(c == '\\'){
                        _was_last_re_dash = true;
                        continue;
                    }

                    // We have to check if we are looking for a list
                    if(c == ',' && !_was_last_re_dash){ 
                        // In first pass, before we knew it was a list we will just add the _final_value to the list
                        _looking_for_list = true;
                        _list_values.Add(_final_value); //  We will add the value to the list
                        _final_value = ""; // We have to reset the _final_value to keep the list values separate
                        _was_last_re_dash = false;
                        continue;
                        // break;
                    }
                    if(c == '!' && !_was_last_re_dash) break; // Comment found without \ before it
                    if(c == '&' && !_was_last_re_dash){ _final_value += ' '; continue;}
                    if((c == '%' && !_was_last_re_dash) || _looking_for_variable){ // No \ before %, or we are already looking for a variable
                        if(c == '%' && !_was_last_re_dash && !_looking_for_variable){_looking_for_variable = true; continue;} // We will skip the % sign and start looking for the variable
                        _was_last_re_dash = false;

                        if(c == '%') _looking_for_variable = false; // We have found the end of the variable
                        if(_looking_for_variable){
                            _variable_name += c;
                            continue;
                        }
                        // Now we will check if the variable exists, and replace it with it's value
                        if(!_variables.Exists(x => x.Name == _variable_name) && !_private_variable.Exists(x => x.Name == _variable_name)){
                            throw new System.IO.IOException("Variable " + _variable_name + " not found at line " + _line_number);
                        }
                        string _variable_value = _variables.Find(x => x.Name == _variable_name).Value; // Check from public variables first
                        _variable_value ??= _private_variable.Find(x => x.Name == _variable_name).Value; // If not found, check from private variables
                        if(_variable_value == null) throw new System.IO.IOException("Variable " + _variable_name + " not found at line " + _line_number);

                        // We have to take into account if we are looking for a list
                        if(_looking_for_list) {_list_values.Add(_variable_value); _final_value = "";} // We have to reset the _final_value to keep the list values separate
                        else _final_value += _variable_value;

                        // Return _variable_name to it's original state
                        _variable_name = "";
                        continue; // We will skip the % sign
                    }

                    if(_was_last_re_dash && c!='!' && c!='%' && c!=',' && c!='&'){
                        _final_value += '\\'; // Add the \ to the final value
                    } 
                    _final_value += c;
                    _was_last_re_dash = false;
                }
                
                if(_looking_for_list) {
                    // In lists, to see the last non-variable value, 
                    // would require the list to look like this: value1, value2, value3, value4,
                    // This is why some tricks are needed
                    if(_final_value.Length > 0) _list_values.Add(_final_value); // Add the last value to the list
                    return _list_values.ToArray();
                }
                else return _final_value;
            }


            // Make these out of the function for parser
            private List<ATRCVariable> _variables = [];
            private List<ATRCVariable> _private_variable = [];
            /// <summary>
            /// Parse the contents of the file
            /// </summary>
            /// <param name="p_filename">File location/name</param>
            /// <returns>block data and variable data</returns>
            private (ATRCBlock[], ATRCVariable[]) ParseFile(string p_filename) {
                List<ATRCBlock> _blocks = [];
                _variables.Clear();
                _private_variable.Clear();
                
                int _start_looking_for_value = 0;
                string _value_string = "";
                string[] _value_string_array = [];

                _was_last_private_variable = false;
                _was_last_variable = false;
                _next_value_symbol = false;
                _was_last_re_dash = false;

                string _block_name = "";
                string _prev_block_name = "";
                int index = 0;
                List<ATRCKey> _keys_buffer = new();
                using (StreamReader linesRead = new(p_filename)){
                while (!linesRead.EndOfStream) {
                    index++;
                    string _line = linesRead.ReadLine(); // Read the next line from the file
                    string _line_trim = _line.Trim();
                    if(_line_trim.Length == 0) continue; // Skip empty lines
                    if(_line_trim.StartsWith('!')) continue; // Skip comments
                    // Check variables first
                    if(_line_trim.StartsWith('%') || _line_trim.StartsWith("<%")){
                        if(_line_trim.StartsWith("<%")){
                            _was_last_private_variable = true;
                        } else {
                            _was_last_variable = true;
                        }

                        // Once we know variable type, we can start looking for the value
                        _start_looking_for_value = _line_trim.IndexOf('=');
                        if(_start_looking_for_value == -1) throw new System.IO.IOException("Invalid variable declaration - No value assigned at line " + index);

                        // Get variable name
                        string _variable_name = "";
                        if(_was_last_private_variable) {
                            _variable_name = _line_trim.AsSpan(2, _line_trim.IndexOf('%', 2) - 2).ToString(); // No idea why this works with -2
                        } else if(_was_last_variable){
                            _variable_name = _line_trim.AsSpan(1, _line_trim.IndexOf('%', 1) - 1).ToString();
                        } 
                        if(_variable_name.Length == 0) throw new System.IO.IOException("Invalid variable declaration - Length is zero at line " + index);
                        if(_variable_name == "!") throw new System.IO.IOException("Invalid variable declaration - Reserved keyword '!' used as variable name at line " + index);
                        // Since ParseValue can return string or string[], we need to check the return type
                        object result = ParserValue(_line, index);
                        if(result is string){
                            _value_string = (string)result;
                        }
                        else if(result is string[]){
                            _value_string_array = (string[])result;
                        }
                        else throw new System.IO.IOException("Invalid variable declaration - Value type not recognized at line " + index);
                        // We will check if the variable is already in the list
                        if(_variables.Exists(x => x.Name == _variable_name) || _private_variable.Exists(x => x.Name == _variable_name)){
                            throw new System.IO.IOException("Variable " + _variable_name + " redefined in file at line " + index);
                        }
                        
                        Filehandler.ATRCVariable _variable = new(){
                            Name = _variable_name,
                            Value = _value_string, // We will assign the value here
                            IsArray = _value_string_array.Length > 0, // If the array is empty, IsArray will be false
                            ArrayValues = _value_string_array, // We will assign the array values here, even if it is empty
                        };
                        // We also have to reset them
                        _value_string = "";
                        _value_string_array = [];
                        // Add them to the correct list
                        if(_was_last_private_variable){
                            _private_variable.Add(_variable);
                        } else {
                            _variables.Add(_variable);
                        }

                        // Reset variables
                        _was_last_private_variable = false;
                        _was_last_variable = false;
                        continue;
                    }
                    
                    // Check for blocks
                    
                    if(_line_trim.StartsWith('[')){
                        _prev_block_name = _block_name;

                        // We will make the block with it's stuff here
                        if(_block_name.Length != 0){ // This needs to be done since _block_name is empty on first run
                            ATRCBlock _block_in = new(){
                                Name = _block_name,
                                Keys = _keys_buffer.ToArray(),
                            };
                            _blocks.Add(_block_in);
                            _keys_buffer.Clear();
                        }

                        _block_name = _line_trim.Substring(1, _line_trim.IndexOf(']') - 1);
                        if(_blocks.Exists(x => x.Name == _block_name) || _block_name == _prev_block_name){
                            throw new System.IO.IOException("Block " + _block_name + " redefined in file at line " + index);
                        } else if(_block_name.Length == 0) {
                            throw new System.IO.IOException("Empty block name declared at line " + index);
                        }
                        continue;
                    }

                    // At this point, we should assume it is a key, since we have checked for variables and blocks
                    if(_block_name.Length == 0) throw new System.IO.IOException("Block name not found at line " + index);
                    string _key_name = _line_trim.Substring(0, _line_trim.IndexOf('='));
                    _key_name = _key_name.Trim(); // trim the key name
                    // Since ParseValue can return string or string[], we need to check the return type
                    object _return_value = ParserValue(_line, index);
                    if(_return_value is string){
                        _value_string = (string)_return_value;
                    } else if(_return_value is string[] v1){
                        _value_string_array = (string[])_return_value;
                    } else {
                        throw new System.IO.IOException("Invalid key value at line " + index);
                    }

                    // We will check if the key already exists in the buffer
                    if(_keys_buffer.Exists(x => x.Name == _key_name)){
                        throw new System.IO.IOException("Key " + _key_name + " redefined in block " + _block_name + " at line " + index);
                    }
                    
                    ATRCKey _key = new(){
                        Name = _key_name,
                            Value = _value_string, // We will assign the value here
                            IsArray = _value_string_array.Length > 0, // If the array is empty, IsArray will be false
                            ArrayValues = _value_string_array, // We will assign the array values here, even if it is empty
                    };
                    // We will append to the last block, since no duplicates or redefinitions are allowed
                    _keys_buffer.Add(_key);
                }
                }
                // This is not pretty, but we have to add the last block
                ATRCBlock _block = new(){
                    Name = _block_name,
                    Keys = _keys_buffer.ToArray(),
                };
                _blocks.Add(_block);
                _keys_buffer.Clear();

                
                return (_blocks.ToArray(), _variables.ToArray());
            }

            /// <summary>
            /// Open a file, set filepath first using Filename property
            /// </summary>
            /// <returns>
            /// ATRCFileData object
            /// </returns>
            public void Read(string p_filename)
            {
                ArgumentNullException.ThrowIfNull(p_filename);
                if (System.IO.Path.GetExtension(p_filename) != ".atrc")
                    throw new System.IO.IOException("File is not an ATRC file");

                // Check if file exists, else create it
                if (!System.IO.File.Exists(p_filename))
                    throw new System.IO.IOException("File does not exist");
                Filename = p_filename;
                (Blocks, Variables) = ParseFile(p_filename);
            }
        }
    }
}