using System;
using System.Drawing;

using AVG.Automation.ZEN;

namespace AVG.Automation.ZEN
{
    /// <summary>
    /// Compares each two consecutive Bitmaps from BitmapBatch object.
    /// </summary>
    public class BitmapBatchComparator
    {
        private BitmapBatch         bitmapBatch;
        private BitmapDifference    bitmapDifference;

        public BitmapBatchComparator(BitmapBatch batch)
        {
            bitmapBatch = batch;

            // Check number of Bitmap objects in BitmapBatch
            if (bitmapBatch.Count < 2)
            {
                throw new Exception("BitmapBatch object needs to contain at least two Bitmap objects");
            }

            // Initialize BitmapDifference holder
            bitmapDifference = new BitmapDifference(bitmapBatch[0]);

            Compare();
        }

        private void Compare()
        {
            BitmapEditor bitmapPrevious = bitmapBatch[0];
            BitmapEditor bitmapCurrent;

            for (int bitmapBatchIndex = 0; bitmapBatchIndex < bitmapBatch.Count; ++bitmapBatchIndex)
            {
                bitmapCurrent = bitmapBatch[bitmapBatchIndex];
                BitmapComparator bitmapComparatorCurrent = new BitmapComparator(bitmapPrevious, bitmapCurrent, bitmapDifference);
            }
        }

        public void AddBitmapDifferenceMask(Bitmap mask)
        {
            bitmapDifference.AddMask(mask);
        }

        public BitmapDifference GetBitmapDifference()
        {
            return bitmapDifference;
        }

        public Bitmap GetBitmapDifferenceImage()
        {
            return bitmapDifference.GetBitmapDifferenceImage();
        }

        public int GetBitmapDifferenceMetric()
        {
            return bitmapDifference.GetBitmapDifferenceMetric();
        }
    }
}
