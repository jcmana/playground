using System;
using System.Drawing;
using System.Drawing.Imaging;

namespace AVG.Automation.ZEN
{
    class BitmapDifferenceMask
    {
        private bool[,]     differenceMask;
        private Size        differenceMaskSize;
        private BitmapPixel differenceMaskColor;

        public BitmapDifferenceMask(int width, int height)
        {
            // Use default to color for ignored pixels
            differenceMaskColor = Color.Cyan;

            // Set mask size
            differenceMaskSize.Width  = width;
            differenceMaskSize.Height = height;

            // Initialize mask of size width x height
            differenceMask = new bool[differenceMaskSize.Width, differenceMaskSize.Height];

            // By default, no pixels will be ignored
            for (int y = 0; y < differenceMaskSize.Height; ++y)
            {
                for (int x = 0; x < differenceMaskSize.Width; ++x)
                {
                    // Initial state will be that all pixels are not masked
                    differenceMask[x, y] = false;
                }
            }
        }

        public BitmapDifferenceMask(Bitmap mask)
        {
            // Use default to color for ignored pixels
            differenceMaskColor = Color.Cyan;

            // Set mask size
            differenceMaskSize.Width = mask.Width;
            differenceMaskSize.Height = mask.Height;

            // Initialize mask of size width x height
            differenceMask = new bool[differenceMaskSize.Width, differenceMaskSize.Height];

            // Fill
            AddMask(mask);
        }

        public bool GetPixel(int x, int y)
        {
            return differenceMask[x, y];
        }

        public void SetPixel(int x, int y, bool value)
        {
            differenceMask[x, y] = value;
        }

        public bool this[int x, int y]
        {
            get { return GetPixel(x, y); }
            set { SetPixel(x, y, value); }
        }

        public void AddMask(BitmapEditor mask)
        {
            // Check Bitmap size, we can't continue if sizes doesn't correspond
            if (mask.Size != differenceMaskSize)
            {
                throw new Exception("Passed Bitmap has different size than initialized mask");
            }

            // Fill mask definition according the passed Bitmap
            // (if pixel on Bitmap is of ignored color - differenceMaskColor - mark it as ignored)
            for (int y = 0; y < differenceMaskSize.Height; ++y)
            {
                for (int x = 0; x < differenceMaskSize.Width; ++x)
                {
                    if (mask.GetPixel(x, y) == differenceMaskColor)
                    {
                        differenceMask[x, y] = true;
                    }
                }
            }
        }
    }
}
