using System;
using System.Drawing;
using System.Drawing.Imaging;

namespace gitimgdiff
{
    public class BitmapComparator
    {
        public enum ComparisonResult { Identical, DifferentColor, DifferentSize };

        private Bitmap bitmapA;
        private Bitmap bitmapB;

        // Public result accessors
        public ComparisonResult Result { get; private set; }
        public int PixelDifference { get; private set; }
        public Bitmap ImageDifference { get; private set; }

        public BitmapComparator(Bitmap a, Bitmap b)
        {
            bitmapA = a;
            bitmapB = b;

            Compare();
        }

        private void Compare()
        {
            // Initialize difference value
            ImageDifference = new Bitmap(bitmapA);
            PixelDifference = 0;

            // Compare dimensions of images A and B
            if (bitmapA.Width != bitmapB.Width || bitmapA.Height != bitmapB.Height)
            {
                // We don't know how to compare images of different sizes
                // Return info that image dimensions differs
                Result = ComparisonResult.DifferentSize;
            }
            else
            {
                // Pixel-pixel comparison
                for (int row = 0; row < bitmapA.Height; ++row)
                {
                    for (int col = 0; col < bitmapA.Width; ++col)
                    {
                        if (bitmapA.GetPixel(col, row) != bitmapB.GetPixel(col, row))
                        {
                            // Set pixel to Cyan in difference image
                            ImageDifference.SetPixel(col, row, System.Drawing.Color.Cyan);

                            // Increase pixel difference counter
                            ++PixelDifference;
                        }
                    }
                }

                // Evaluate comparison result
                if (PixelDifference > 0)
                {
                    Result = ComparisonResult.DifferentColor;
                }
                else
                {
                    Result = ComparisonResult.Identical;
                }
            }
        }
    }
}
