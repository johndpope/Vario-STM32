package club.rascal.notorious.aconsole;

import android.util.Log;

/**
 * Created by Jasmine on 2018-01-23.
 */

public class VarioResponse {
    public int mCode;           // Command code ex) 'UP', 'QP', 'RS', ...
    public long mParam;

    public int mDataType;
    public int mDataCount;

    public long[] mDataL;
    public double[] mDataD;
    public String mDataS;

    public static final int DTYPE_NONE = 0;
    public static final int DTYPE_NUMBER = 1;
    public static final int DTYPE_FLOAT = 2;
    public static final int DTYPE_STRING = 3;

    /*
    public static final int RCODE_RESULT		    = 'R' * 256 + 'S';
    public static final int RCODE_OK				= 'O' * 256 + 'K';
    public static final int RCODE_FAIL			    = 'F' * 256 + 'A';
    public static final int RCODE_ERROR			= 'E' * 256 + 'R';
    public static final int RCODE_NOT_READY		= 'N' * 256 + 'R';
    public static final int RCODE_UNAVAILABLE		= 'U' * 256 + 'A';
    public static final int RCODE_DUMP_PARAM		= 'D' * 256 + 'P';
    public static final int RCODE_QUERY_PARAM		= 'Q' * 256 + 'P';
    public static final int RCODE_UPDATE_PARAM	= 'U' * 256 + 'P';
    */

    public static final int RPARAM_SUCCESS = 0;
    public static final int RPARAM_OK = 0;

    public static final int RPARAM_FAIL = 1;
    public static final int RPARAM_INVALID_COMMAND = 2;
    public static final int RPARAM_INVALID_PROPERTY = 3;
    public static final int RPARAM_NOT_READY = 4;
    public static final int RPARAM_UNAVAILABLE = 5;
    public static final int RPARAM_NOT_ALLOWED = 6;
    public static final int RPARAM_INVALID_PARAMETER = 7;
    public static final int RPARAM_INVALID_DATA = 8;

    public static final int RPARAM_CAL_START = (0x1000);
    public static final int RPARAM_CAL_MODE_CHANGED = (0x1001); // INIT->READY->MEASURE->CALIBATE->DONE or STOP
    public static final int RPARAM_CAL_MEASURED_RESULT = (0x1003); // validation, orient, accel standard deviation
    public static final int RPARAM_CAL_DONE = (0x1002)	; // calibration accel x/y/z
    public static final int RPARAM_CAL_ACCELEROMETER = (0x1004);	// calibrated accel x/y/z

    public static final int RPARAM_SW_BASE = (0x2000);
    public static final int RPARAM_SW_VARIO = (0x2001);
    public static final int RPARAM_SW_UMS = (0x2002);
    public static final int RPARAM_SW_CALIBRATION = (0x2003);


    public static final int MAX_DATACOUNT   = 4;

    private VarioResponse(int code) {
        mCode = code;
        mParam = 0xFFFFFFFF;

        mDataType = DTYPE_NONE;
        mDataCount = 0;

        mDataL = null;
        mDataD = null;
        mDataS = null;
    }

    public String toString() {
        StringBuilder sb = new StringBuilder(32);

        // code
        sb.append('%');
        sb.append((char)(mCode / 256));
        sb.append((char)(mCode % 256));

        // param
        if (mParam < 0xFFFFFFFF) {
            sb.append(',');
            sb.append(mParam);

            if (mDataType == DTYPE_NUMBER) {
                for (int i = 0; i < mDataCount; i++) {
                    sb.append(',');
                    sb.append(mDataL[i]);
                }
            } else if (mDataType == DTYPE_FLOAT) {
                for (int i = 0; i < mDataCount; i++) {
                    sb.append(',');
                    sb.append(mDataD[i]);
                }
            } else if (mDataType == DTYPE_STRING) {
                sb.append(',');
                sb.append(mDataS);
            }
        }

        return sb.toString();
    }
    
    public int getInteger() {
        switch (mDataType) {
            case DTYPE_NUMBER :
                return (int)mDataL[0];
            case DTYPE_FLOAT :
                return (int)mDataD[0];
            case DTYPE_STRING :
                return Integer.parseInt(mDataS);
        }
        
        return 0;
    }
    
    public float getFloat() {
        switch (mDataType) {
            case DTYPE_NUMBER :
                return (float)mDataL[0];
            case DTYPE_FLOAT :
                return (float)mDataD[0];
            case DTYPE_STRING :
                return Float.parseFloat(mDataS);
        }

        return 0;
    }
    
    public String getString() {
        switch (mDataType) {
            case DTYPE_NUMBER :
                return Long.toString(mDataL[0]);
            case DTYPE_FLOAT :
                return Double.toString(mDataD[0]);
            case DTYPE_STRING :
                return mDataS;
        }

        return "";
    }

    //
    public static int verifyType(String str) {
        int type = DTYPE_NONE;

        for (int i = 0; i < str.length() && type != DTYPE_STRING; i++) {
            int c = str.charAt(i);

            switch (type)  {
                case DTYPE_NONE		:
                    if (('0' <= c && c <= '9') || c == '-')
                        type = DTYPE_NUMBER;
                    else
                        type = DTYPE_STRING;
                    break;
                case DTYPE_NUMBER	:
                    if (c == '.')
                        type = DTYPE_FLOAT;
                    else if (c < '0' || '9' < c)
                        type = DTYPE_STRING;
                    break;
                case DTYPE_FLOAT		:
                    if (c < '0' || '9' < c)
                        type = DTYPE_STRING;
                    break;
            }
        }

        return type;
    }

    public static VarioResponse parse(String line) {
        // %{CODE}[,{PARAM}[,{VALUE1}[,{VALUE2}[,{VALUE3}[,{VALUE4}]]]]]\r\n
        // %{CODE}[,{PARAM}[,{STRING}]]\r\n
        String[] tokens = line.split(",");
        int tokCount = tokens.length;

        if (tokens[0].length() != 3)
            return null;

        //
        VarioResponse res = new VarioResponse(tokens[0].charAt(1) * 256 + tokens[0].charAt(2));

        if (tokCount > 1) {
            res.mParam = Long.parseLong(tokens[1]);

            if (tokCount > 2) {
                int dataCount = tokCount > 6 ? 4 : tokCount - 2;
                int dataType = verifyType(tokens[2]);

                for (int i = 1; i < dataCount; i++) {
                    int type = verifyType(tokens[2+i]);

                    // n+n->n, n+f->f, f+s->s
                    if (dataType < type )
                        dataType = type;
                }

                /*
                for (int i = 0; i < dataCount; i++) {
                    switch (dataType) {
                        case DTYPE_NUMBER :
                            res.mDataL
                        case DTYPE_FLOAT :
                        case DTYPE_STRING :
                    }
                 */

                res.mDataCount = dataCount; // we treat first value only -> ignore others
                res.mDataType = dataType;

                switch (res.mDataType) {
                    case DTYPE_NUMBER :
                        res.mDataL = new long[res.mDataCount];
                        for (int i = 0; i < dataCount; i++)
                            res.mDataL[i] = Long.parseLong(tokens[2+i]);
                        break;
                    case DTYPE_FLOAT :
                        res.mDataD = new double[res.mDataCount];
                        for (int i = 0; i < dataCount; i++)
                            res.mDataD[i] = Double.parseDouble(tokens[2+i]);
                        break;
                    case DTYPE_STRING :
                        StringBuilder sb = new StringBuilder(32);
                        for (int i = 0; i  < tokCount - 2; i++) {
                            // if string value has comma, ...
                            sb.append(tokens[2+i]);
                        }
                        res.mDataS = sb.toString();
                        res.mDataCount = 1;
                        break;
                }
            }
        }

        return res;
    }
}