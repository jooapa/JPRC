namespace ATRC
{
    public partial class Filehandler{

        public struct ATRCVariable{
            public string Name;
            public string Value;
        }
        public struct ATRCKey{
            public string Name;
            public string Value;
        }
        public struct ATRCBlock{
            public string Name;
            public ATRCKey[] Keys;
        }
        public struct ATRCFileData{
            public string Filename;
            public ATRCVariable[] Variables;
            public ATRCBlock[] Blocks;
        }
    }

}