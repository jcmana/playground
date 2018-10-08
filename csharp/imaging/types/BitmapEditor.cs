using System;
using System.Drawing;
using System.Drawing.Imaging;

namespace AVG.Automation.ZEN
{
    unsafe public class BitmapEditor
    {
        // / / Private internal properties
        private Bitmap      editorSource;
        private BitmapData  editorSourceData;
        private Byte *      editorSourceBytes;

        private Int32       editorSourceStride;     // Number of bytes on a single row of image (including alignment bytes)
        private Int32       editorSourceBPP;        // Bytes Per Pixel
        private Int32       editorSourceOffsetR;    // Memory offset of pixel component R
        private Int32       editorSourceOffsetG;
        private Int32       editorSourceOffsetB;
        private Int32       editorSourceOffsetA;


        // / / Public methods

        public BitmapEditor(Bitmap bitmap)
        {
            // Store original Bitmap object
            editorSource = new Bitmap(bitmap);

            // Get BitmapData object from original Bitmap
            editorSourceData = editorSource.LockBits((new Rectangle(0, 0, editorSource.Width, editorSource.Height)), ImageLockMode.ReadOnly, editorSource.PixelFormat);

            // Get pointer to bytes of BitmapData objects
            editorSourceBytes = (Byte*)(editorSourceData.Scan0.ToPointer());

            // Get Bitmap stride
            editorSourceStride = editorSourceData.Stride;

            // Resolve BPP and pixel components offsets of the Bitmap
            switch (editorSource.PixelFormat)
            {
                case PixelFormat.Format24bppRgb:
                    editorSourceBPP = 3;

                    editorSourceOffsetR = 2;
                    editorSourceOffsetG = 1;
                    editorSourceOffsetB = 0;
                    editorSourceOffsetA = -1;

                    break;

                case PixelFormat.Format32bppArgb:
                    editorSourceBPP = 4;

                    editorSourceOffsetR = 2;
                    editorSourceOffsetG = 1;
                    editorSourceOffsetB = 0;
                    editorSourceOffsetA = 3;

                    break;

                default:
                    throw new Exception("Unhandled pixel format " + editorSource.PixelFormat);
            }

            // Tunnel properties from Bitmap object
            Size = editorSource.Size;
            Width = editorSource.Width;
            Height = editorSource.Height;
            PixelFormat = editorSource.PixelFormat;
        }

        public BitmapPixel GetPixel(int x, int y)
        {
            int pixelOffset = GetPixelOffset(x, y);

            // A component - not all formats have Alpha component
            Byte A;
            if (editorSourceOffsetA >= 0)
            {
                A = editorSourceBytes[pixelOffset + editorSourceOffsetA];
            }
            else
            {
                A = 255;
            }

            // R component
            Byte R = editorSourceBytes[pixelOffset + editorSourceOffsetR];

            // G component
            Byte G = editorSourceBytes[pixelOffset + editorSourceOffsetG];

            // B component
            Byte B = editorSourceBytes[pixelOffset + editorSourceOffsetB];

            // Compose and return Color from retrieved pixel components
            return new BitmapPixel(A, R, G, B);
        }

        public void SetPixel(int x, int y, BitmapPixel value)
        {
            int pixelOffset = GetPixelOffset(x, y);

            // A component - not all formats have Alpha component
            if (editorSourceOffsetA >= 0)
            {
                editorSourceBytes[pixelOffset + editorSourceOffsetA] = value.A;
            }
            
            // R component
            editorSourceBytes[pixelOffset + editorSourceOffsetR] = value.R;

            // G component
            editorSourceBytes[pixelOffset + editorSourceOffsetG] = value.G;

            // B component
            editorSourceBytes[pixelOffset + editorSourceOffsetB] = value.B;
        }


        public Bitmap GetBitmap()
        {
            // Unlock Bitmap bits
            editorSource.UnlockBits(editorSourceData);

            return editorSource;
        }



        // / / Public getters, private setters, tunneled from Bitmap object
        public Size         Size        { get; private set; }

        public int          Width       { get; private set; }

        public int          Height      { get; private set; }

        public PixelFormat  PixelFormat { get; private set; }


        // / / Public type conversion operators

        static public implicit operator BitmapEditor(Bitmap value)
        {
            return new BitmapEditor(value);
        }

        static public implicit operator Bitmap(BitmapEditor value)
        {
            return value.GetBitmap();
        }


        // / / Private internal methods

        private int GetPixelOffset(int x, int y)
        {
            return ((y * editorSourceStride) + (x * editorSourceBPP));
        }
    }
}
