using System;
using System.Drawing;

namespace AVG.Automation.ZEN
{
    /// <summary>
    /// BitmapPixel class respresents lightweight version of System.Drawing.Color. It was designed to allow
    /// fast pixel color construction and comparison.
    /// </summary>
    public class BitmapPixel : Object
    {
        public Byte A { get; set; }
        public Byte R { get; set; }
        public Byte G { get; set; }
        public Byte B { get; set; }

        public BitmapPixel(Byte a, Byte r, Byte g, Byte b)
        {
            A = a;
            R = r;
            G = g;
            B = b;
        }

        public BitmapPixel(Byte r, Byte g, Byte b) : this(255, r, g, b)
        {

        }

        public BitmapPixel() : this(255, 0, 0, 0)
        {

        }

        public override bool Equals(Object operand)
        {
            if (operand == null) return false;

            BitmapPixel bitmapPixeloperand = operand as BitmapPixel;

            if ((Object)(bitmapPixeloperand) == null) return false;

            return Equals(bitmapPixeloperand);
        }

        public bool Equals(BitmapPixel operand)
        {
            if (operand == null) return false;

            return (A == operand.A) && (R == operand.R) && (G == operand.G) && (B == operand.B);
        }

        public override string ToString()
        {
            return "R:" + R + " G:" + G + " B:" + B + " A:" + A;
        }

        public static bool operator == (BitmapPixel a, BitmapPixel b)
        {
            if (Object.ReferenceEquals(a, b)) return true;

            if ((Object)(a) == null || (Object)(b) == null) return false;

            return (a.A == b.A) && (a.R == b.R) && (a.G == b.G) && (a.B == b.B);
        }

        public static bool operator != (BitmapPixel a, BitmapPixel b)
        {
            return !(a == b);
        }

        static public implicit operator BitmapPixel(Color value)
        {
            return new BitmapPixel(value.A, value.R, value.G, value.B);
        }

        static public implicit operator Color(BitmapPixel value)
        {
            return Color.FromArgb(value.A, value.R, value.G, value.B);
        }
    }
}
