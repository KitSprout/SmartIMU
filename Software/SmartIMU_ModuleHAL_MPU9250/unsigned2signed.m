function data_int16 = unsigned2signed( data_uint16 )

    if data_uint16 > 2^15 + 1
        data_int16 = int16((bitcmp(uint16(data_uint16)) + 1)) * (-1);
    else
        data_int16 = int16(data_uint16);
    end

end
