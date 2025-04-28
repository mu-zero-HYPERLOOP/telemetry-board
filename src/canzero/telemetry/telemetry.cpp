int64_t canzero::telemetry::getNetworkHash() __pure
{
    return 0;
}

int64_t canzero::telemetry::begin()
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    Timestamp::now();
    Timestamp::operator=(&canzero::telemetry::timebase);
    telemetry_board::TcpServer::start();
    canzero::telemetry::UDP_BROADCAST_PORT;
    telemetry_board::UdpServer::connect(0xba8);
    int16_t var_24 = telemetry_board::TcpServer::welcomePort();
    std::println<uint16_t>(0x1a, "Starting server at port {}");
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t canzero::telemetry::udpBeconLoop()
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    void var_1f8;
    
    if (telemetry_board::UdpServer::recv(&canzero::telemetry::udpServer, &var_1f8, 0xd8))
    {
        int64_t rax_2;
        int64_t rdx_1;
        rax_2 = canzero::telemetry::Datagram::serviceName();
        canzero::telemetry::SERVICE_NAME;
        data_406398;
        
        if (std::operator==<char>(rax_2, rdx_1))
        {
            std::println<>(0x17, "Received Hello datagram");
            int64_t rax_5;
            int64_t rdx_3;
            rax_5 = buildTime();
            uint64_t rax_7 = Timestamp::operator uint32_t();
            uint64_t rsi_3 = telemetry_board::TcpServer::welcomePort();
            char const (* var_220_1)[0x7] = data_4063a8;
            int64_t canzero::telemetry::SERVER_NAME_1 = canzero::telemetry::SERVER_NAME;
            int64_t var_230_1 = rdx_3;
            int64_t var_238_1 = rax_5;
            void var_118;
            canzero::telemetry::Datagram::createNetworkDescription(&var_118, rsi_3, 0, rax_7, 
                canzero::telemetry::SERVICE_NAME, data_406398);
            void* var_200;
            telemetry_board::UdpServer::send(&canzero::telemetry::udpServer, var_200, &var_118);
            std::println<>(0x13, "Responding to Hello");
        }
    }
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t canzero::telemetry::tcpServerRxLoop()
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int32_t rax_1 = data_400bc8;
    char var_78;
    
    if (rax_1 == 3)
    {
        int32_t rax_20 =
            telemetry_board::TcpConnection::recv(&canzero::telemetry::connection, &var_78);
        
        if (rax_20 == 2)
        {
            char rax_21 = var_78;
            
            if (rax_21 == 2)
                canzero::telemetry::CanzeroConnection::close();
            else if (rax_21 <= 2 && rax_21 && rax_21 == 1)
            {
                int32_t var_74;
                int32_t var_58_1 = var_74;
                char var_76;
                char var_54_1 = var_76;
                int64_t var_68;
                int64_t var_53_1 = var_68;
                char var_77;
                
                if (!var_77)
                    CyclicBuffer<canzero_frame, 32ul>::enqueue(&canzero::telemetry::can0_rxQueue);
                else if (var_77 != 1)
                    canzero::telemetry::CanzeroConnection::close();
                else
                    CyclicBuffer<canzero_frame, 32ul>::enqueue(&canzero::telemetry::can1_rxQueue);
            }
        }
        else if (rax_20 <= 2 && !rax_20)
            canzero::telemetry::CanzeroConnection::close();
    }
    else if (rax_1 == 2)
    {
        int32_t rax_19 =
            telemetry_board::TcpConnection::send(&canzero::telemetry::connection, &data_400bcc);
        std::println<>(0x1d, "Retry to send option response");
        
        if (rax_19 == 2)
            data_400bc8 = 3;
    }
    else if (!rax_1)
    {
        telemetry_board::TcpServer::accept();
        
        if (std::optional<telemetry_board::TcpConnection>::has_value())
        {
            std::println<>(0xe, "New connection");
            telemetry_board::TcpConnection::operator=(&canzero::telemetry::connection);
            data_400bc8 = 1;
        }
        
        std::optional<telemetry_board::TcpConnection>::~optional();
    }
    else if (rax_1 == 1)
    {
        int32_t rax_5 =
            telemetry_board::TcpConnection::recv(&canzero::telemetry::connection, &var_78);
        
        if (rax_5 == 2)
        {
            if (!(var_78 & 0xf))
            {
                if (var_78 & 0x80 || var_78 & 0x40)
                {
                    bool var_8a_1 = true;
                    uint8_t var_89_1 = 0;
                    
                    if (var_78 & 0x40)
                        var_8a_1 = false;
                    
                    if (var_78 & 0x80)
                    {
                        var_89_1 = 0xd;
                        var_8a_1 = false;
                    }
                    
                    data_400bcc = canzero::telemetry::HandshakePacket::createOptionResponse(
                        var_8a_1, var_89_1);
                    int32_t rax_18 = telemetry_board::TcpConnection::send(
                        &canzero::telemetry::connection, &data_400bcc);
                    std::println<>(0x1e, "Trying to send option response");
                    
                    if (rax_18 == 2)
                        data_400bc8 = 3;
                    else if (!rax_18)
                        canzero::telemetry::CanzeroConnection::close();
                    else if (rax_18 == 1)
                        data_400bc8 = 2;
                }
                else
                    data_400bc8 = 3;
            }
        }
        else if (rax_5 <= 2 && !rax_5)
            canzero::telemetry::CanzeroConnection::close();
    }
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t sub_400613(void* arg1 @ rbp, int64_t arg2, int64_t arg3, int64_t arg4, int64_t arg5, int64_t arg6, int64_t arg7)
{
    std::optional<telemetry_board::TcpConnection>::~optional();
    void* fsbase;
    
    if (*(arg1 - 0x38) != *(fsbase + 0x28))
    {
        __stack_chk_fail();
        /* no return */
    }
    
    _Unwind_Resume();
    int64_t rax_1 = *(arg1 - 0x38);
    
    if (rax_1 == *(fsbase + 0x28))
        return rax_1 - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t canzero::telemetry::processTxQueue(CyclicBuffer<Timestamped<canzero_frame>, 32ul>& arg1, uint8_t arg2)
{
    void* fsbase;
    int64_t rax_1 = *(fsbase + 0x28);
    void var_68;
    canzero::telemetry::FramePacket::createNetworkFrame(&var_68, arg2, 0, 0, 0);
    int128_t s;
    __builtin_memset(&s, 0, 0x18);
    
    while (true)
    {
        CyclicBuffer<Timestamped<canzero_frame>, 32ul>::dequeue();
        std::optional<Timestamped<canzero_frame> >::operator=(&s);
        
        if (!std::optional<Timestamped<canzero_frame> >::has_value())
            break;
        
        std::optional<Timestamped<canzero_frame> >::value();
        int32_t* rax_3 = Timestamped<canzero_frame>::value();
        int64_t var_58_1 = *(rax_3 + 5);
        int32_t var_64_1 = *rax_3;
        char var_66_1 = rax_3[1];
        std::optional<Timestamped<canzero_frame> >::operator->();
        Timestamped<canzero_frame>::timestamp();
        void var_78;
        int32_t var_7c_1 = operator-(&var_78, &canzero::telemetry::timebase);
        uint64_t var_60_1 = Duration::as_ms();
        int32_t rax_15 =
            telemetry_board::TcpConnection::send(&canzero::telemetry::connection, &var_68);
        
        if (rax_15 != 2 && rax_15 <= 2)
        {
            if (!rax_15)
                canzero::telemetry::CanzeroConnection::close();
            else
            {
                if (rax_15 == 1)
                {
                    std::optional<Timestamped<canzero_frame> >::operator*();
                    CyclicBuffer<Timestamped<canzero_frame>, 32ul>::enqueue(arg1);
                    break;
                }
            }
        }
    }
    
    if (rax_1 == *(fsbase + 0x28))
        return rax_1 - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

void* canzero::telemetry::tcpServerTxLoop()
{
    if (data_400bc8 == 3)
    {
        canzero::telemetry::processTxQueue(&canzero::telemetry::can0_txQueue, 0);
        return canzero::telemetry::processTxQueue(&canzero::telemetry::can1_txQueue, 1);
    }
    
    CyclicBuffer<Timestamped<canzero_frame>, 32ul>::clear();
    return CyclicBuffer<Timestamped<canzero_frame>, 32ul>::clear();
}

void* canzero::telemetry::tcpServerLoop()
{
    canzero::telemetry::tcpServerRxLoop();
    return canzero::telemetry::tcpServerTxLoop();
}

void* canzero::telemetry::update()
{
    canzero::telemetry::udpBeconLoop();
    return canzero::telemetry::tcpServerLoop();
}

uint64_t canzero::telemetry::can0_recv(canzero_frame* arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    CyclicBuffer<canzero_frame, 32ul>::dequeue();
    
    if (std::optional<canzero_frame>::has_value())
    {
        int64_t* rax_2 = std::optional<canzero_frame>::operator*();
        int64_t rdx_1 = rax_2[1];
        *arg1 = *rax_2;
        *(arg1 + 8) = rdx_1;
    }
    
    uint64_t result = std::optional<canzero_frame>::has_value();
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

uint64_t canzero::telemetry::can1_recv(canzero_frame* arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    CyclicBuffer<canzero_frame, 32ul>::dequeue();
    
    if (std::optional<canzero_frame>::has_value())
    {
        int64_t* rax_2 = std::optional<canzero_frame>::operator*();
        int64_t rdx_1 = rax_2[1];
        *arg1 = *rax_2;
        *(arg1 + 8) = rdx_1;
    }
    
    uint64_t result = std::optional<canzero_frame>::has_value();
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

int64_t canzero::telemetry::can0_send(canzero_frame* arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    *arg1;
    *(arg1 + 8);
    void var_28;
    Timestamped<canzero_frame>::now(&var_28);
    int64_t result =
        CyclicBuffer<Timestamped<canzero_frame>, 32ul>::enqueue(&canzero::telemetry::can0_txQueue);
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

int64_t canzero::telemetry::can1_send(canzero_frame* arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    *arg1;
    *(arg1 + 8);
    void var_28;
    Timestamped<canzero_frame>::now(&var_28);
    int64_t result =
        CyclicBuffer<Timestamped<canzero_frame>, 32ul>::enqueue(&canzero::telemetry::can1_txQueue);
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

int64_t __static_initialization_and_destruction_0()
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    telemetry_board::UdpServer::UdpServer(&canzero::telemetry::udpServer);
    __cxa_atexit(*telemetry_board::UdpServer::~UdpServer, &canzero::telemetry::udpServer, 
        __dso_handle);
    uint64_t var_18 = 2;
    uint64_t var_20 = 0x18;
    telemetry_board::TcpServer::TcpServer(&canzero::telemetry::tcpServer, 
        *std::max<uint64_t>(&var_20, &var_18));
    __cxa_atexit(*telemetry_board::TcpServer::~TcpServer, &canzero::telemetry::tcpServer, 
        __dso_handle);
    canzero::telemetry::CanzeroConnection::CanzeroConnection();
    __cxa_atexit(canzero::telemetry::CanzeroConnection::~CanzeroConnection, 
        &canzero::telemetry::connection, __dso_handle);
    CyclicBuffer<canzero_frame, 32ul>::CyclicBuffer();
    CyclicBuffer<canzero_frame, 32ul>::CyclicBuffer();
    CyclicBuffer<Timestamped<canzero_frame>, 32ul>::CyclicBuffer();
    CyclicBuffer<Timestamped<canzero_frame>, 32ul>::CyclicBuffer();
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t (static initializer)::canzero::telemetry::getNetworkHash()
{
    return __static_initialization_and_destruction_0();
}

int64_t std::is_constant_evaluated() __pure
{
    return 0;
}

uint64_t std::operator==(std::strong_ordering arg1, std::__cmp_cat::__unspec arg2) __pure
{
    uint64_t result;
    result = !arg1;
    return result;
}

uint64_t std::operator<(std::strong_ordering arg1, std::__cmp_cat::__unspec arg2) __pure
{
    uint64_t result;
    result = arg1 >> 7;
    return result;
}

void** std::exception::exception()
{
    void** result;
    *result = (_vtable_for_std::exception + 0x10);
    return result;
}

void* operator new(uint64_t arg1, void* arg2) __pure
{
    uint64_t var_10 = arg1;
    return arg2;
}

int64_t operator delete(void* arg1, void* arg2) __pure
{
    void* var_10 = arg1;
    void* var_18 = arg2;
}

void* std::char_traits<char>::assign(char& arg1, char const& arg2)
{
    void* result = arg1;
    *result = *arg2;
    return result;
}

uint64_t std::char_traits<char>::eq(char const& arg1, char const& arg2)
{
    uint64_t result;
    result = *arg1 == *arg2;
    return result;
}

uint64_t std::char_traits<char>::lt(char const& arg1, char const& arg2)
{
    uint64_t result;
    result = *arg1 < *arg2;
    return result;
}

int64_t std::char_traits<char>::compare(char const* arg1, char const* arg2, uint64_t arg3)
{
    if (arg3)
        return memcmp(arg1, arg2, arg3);
    
    return 0;
}

int64_t std::char_traits<char>::length(char const* arg1)
{
    return strlen(arg1);
}

void* std::char_traits<char>::find(char const* arg1, uint64_t arg2, char const& arg3)
{
    if (arg2)
        return memchr(arg1, *arg3, arg2);
    
    return nullptr;
}

char* std::char_traits<char>::move(char* arg1, char const* arg2, uint64_t arg3)
{
    if (arg3)
        return memmove(arg1, arg2, arg3);
    
    return arg1;
}

char* std::char_traits<char>::copy(char* arg1, char const* arg2, uint64_t arg3)
{
    if (arg3)
        return memcpy(arg1, arg2, arg3, arg1);
    
    return arg1;
}

char* std::char_traits<char>::assign(char* arg1, uint64_t arg2, char arg3)
{
    if (arg2)
        return memset(arg1, arg3, arg2);
    
    return arg1;
}

int64_t __ops::__iter_less_val() __pure
{
    return;
}

int64_t __ops::__val_less_iter() __pure
{
    return;
}

uint64_t std::__sv_check(uint64_t arg1, uint64_t arg2, char const* arg3)
{
    if (arg1 < arg2)
        std::__throw_out_of_range_fmt("%s: __pos (which is %zu) > __siz…", arg3, arg2, arg1);
    
    return arg2;
}

char* std::string::_M_local_data()
{
    void* rdi;
    return std::__ptr_traits_ptr_to<char*, char, false>::pointer_to(rdi + 0x10);
}

char* std::__ptr_traits_ptr_to<char*, char, false>::pointer_to(char& arg1)
{
    return std::addressof<char>(arg1);
}

char* std::addressof<char>(char& arg1)
{
    return std::__addressof<char>(arg1);
}

char& std::__addressof<char>(char& arg1) __pure
{
    return arg1;
}

char* std::string::_Alloc_hider::_Alloc_hider(char* arg1, std::allocator<char>&& arg2)
{
    char* var_18 = arg1;
    std::allocator<char>** rdx;
    std::allocator<char>** var_10 = std::move<std::allocator<char>&>(rdx);
    *arg1 = arg2;
    return arg1;
}

std::allocator<char>&&& std::move<std::allocator<char>&>(std::allocator<char>&&& arg1) __pure
{
    return arg1;
}

int64_t std::string::_Alloc_hider::~_Alloc_hider() __pure
{
    int64_t result;
    int64_t result_1 = result;
    return result;
}

int64_t std::string::string()
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    void var_39;
    void* var_28 = &var_39;
    char* rdi;
    std::string::_Alloc_hider::_Alloc_hider(rdi, std::string::_M_local_data());
    std::string::_M_set_length(rdi);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

uint32_t std::__to_chars_len<uint32_t>(uint32_t arg1, int32_t arg2) __pure
{
    uint32_t var_2c = arg1;
    int32_t result = 1;
    int32_t rax_1 = arg2 * arg2;
    int32_t rax_3 = arg2 * rax_1;
    uint64_t rax_6 = arg2 * rax_3;
    
    while (true)
    {
        if (var_2c < arg2)
            return result;
        
        if (var_2c < rax_1)
            return result + 1;
        
        if (var_2c < rax_3)
            return result + 2;
        
        if (var_2c < rax_6)
            break;
        
        var_2c = COMBINE(0, var_2c) / rax_6;
        result += 4;
    }
    
    return result + 3;
}

uint32_t std::__to_chars_len<uint64_t>(uint64_t arg1, int32_t arg2) __pure
{
    uint64_t var_30 = arg1;
    int32_t result = 1;
    int32_t rax_1 = arg2 * arg2;
    int32_t rax_3 = arg2 * rax_1;
    uint64_t rax_6 = arg2 * rax_3;
    
    while (true)
    {
        if (var_30 < arg2)
            return result;
        
        if (var_30 < rax_1)
            return result + 1;
        
        if (var_30 < rax_3)
            return result + 2;
        
        if (var_30 < rax_6)
            break;
        
        var_30 = COMBINE(0, var_30) / rax_6;
        result += 4;
    }
    
    return result + 3;
}

uint32_t std::__to_chars_len<uint64_t>(uint64_t arg1, int32_t arg2) __pure
{
    uint64_t var_30 = arg1;
    int32_t result = 1;
    int32_t rax_1 = arg2 * arg2;
    int32_t rax_3 = arg2 * rax_1;
    uint64_t rax_6 = arg2 * rax_3;
    
    while (true)
    {
        if (var_30 < arg2)
            return result;
        
        if (var_30 < rax_1)
            return result + 1;
        
        if (var_30 < rax_3)
            return result + 2;
        
        if (var_30 < rax_6)
            break;
        
        var_30 = COMBINE(0, var_30) / rax_6;
        result += 4;
    }
    
    return result + 3;
}

int64_t canzero::telemetry::Datagram::serviceName()
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    void* rdi;
    *(rdi + 0x18);
    int64_t result;
    std::basic_string_view<char>::basic_string_view(&result, rdi + 0x19);
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::basic_string_view<char>::size()
{
    int64_t* rdi;
    return *rdi;
}

int64_t std::basic_string_view<char>::data()
{
    void* rdi;
    return *(rdi + 8);
}

int64_t std::basic_string_view<char>::empty()
{
    int64_t result;
    int64_t* rdi;
    result = !*rdi;
    return result;
}

char* canzero::telemetry::Datagram::createNetworkDescription(uint16_t arg1, uint64_t arg2, uint64_t arg3, std::basic_string_view<char> arg4, std::basic_string_view<char> arg5, std::basic_string_view<char> arg6)
{
    char* result = arg1;
    int64_t var_68 = arg5;
    int64_t var_60 = arg6;
    memset(result, 0, 0xd8);
    *result = 1;
    *(result + 2) = arg2;
    *(result + 8) = arg3;
    *(result + 0x10) = arg4;
    int64_t rax_6 = std::basic_string_view<char>::size();
    
    if (rax_6 > 0x3e)
    {
        __assert_fail("serviceNameLen < (sizeof(datagra…", "/home/kistenklaus/Documents/muze…", 
            0x43, "static canzero::telemetry::Datag…");
        /* no return */
    }
    
    result[0x18] = rax_6;
    memcpy(&result[0x19], std::basic_string_view<char>::data(), rax_6 + 1);
    
    if (std::basic_string_view<char>::empty() != 1)
    {
        int64_t rax_15 = std::basic_string_view<char>::size();
        
        if (rax_15 > 0x3e)
        {
            __assert_fail("buildTimeLen < (sizeof(datagram.…", "/home/kistenklaus/Documents/muze…", 
                0x4a, "static canzero::telemetry::Datag…");
            /* no return */
        }
        
        result[0x58] = rax_15;
        memcpy(&result[0x59], std::basic_string_view<char>::data(), rax_15 + 1);
    }
    
    int64_t rax_22 = std::basic_string_view<char>::size();
    
    if (rax_22 > 0x3e)
    {
        __assert_fail("serverNameLen < (sizeof(datagram…", "/home/kistenklaus/Documents/muze…", 
            0x50, "static canzero::telemetry::Datag…");
        /* no return */
    }
    
    result[0x98] = rax_22;
    memcpy(&result[0x99], std::basic_string_view<char>::data(), rax_22 + 1);
    return result;
}

uint64_t canzero::telemetry::HandshakePacket::createOptionResponse(bool arg1, uint8_t arg2)
{
    uint8_t var_11 = arg2;
    void* fsbase;
    *(fsbase + 0x28);
    char var_12;
    
    if (*(fsbase + 0x28) == *(fsbase + 0x28))
        return ((((var_12 & 0xf0) | 1) & 0x7f) | arg1 << 7);
    __stack_chk_fail();
    /* no return */
}

char* canzero::telemetry::FramePacket::createNetworkFrame(uint8_t arg1, uint8_t arg2, uint16_t arg3, uint64_t arg4, uint64_t arg5)
{
    char* result = arg1;
    *result = 1;
    result[1] = arg2;
    result[2] = arg3;
    *(result + 4) = arg4;
    *(result + 8) = arg5;
    int64_t r9;
    *(result + 0x10) = r9;
    return result;
}

int64_t std::bad_optional_access::what() __pure
{
    int64_t rdi;
    int64_t var_10 = rdi;
    return "bad optional access";
}

void*** std::bad_optional_access::bad_optional_access()
{
    std::exception::exception();
    void*** result;
    *result = &data_411260;
    return result;
}

int64_t std::bad_optional_access::~bad_optional_access()
{
    void*** rdi;
    *rdi = &data_411260;
    return std::exception::~exception();
}

int64_t std::bad_optional_access::~bad_optional_access()
{
    std::bad_optional_access::~bad_optional_access();
    void* rdi;
    return operator delete(rdi, 8);
}

void std::__throw_bad_optional_access() __noreturn
{
    int64_t* thrown_exception = __cxa_allocate_exception(8);
    *thrown_exception = 0;
    std::bad_optional_access::bad_optional_access();
    __cxa_throw(thrown_exception, &_typeinfo_for_std::bad_optional_access, 
        std::bad_optional_access::~bad_optional_access);
    /* no return */
}

int64_t* telemetry_board::TcpConnection::TcpConnection()
{
    int64_t* result;
    *result = 0;
    return result;
}

telemetry_board::TcpConnection&& telemetry_board::TcpConnection::operator=(telemetry_board::TcpConnection&& arg1)
{
    int64_t* rsi;
    
    if (arg1 == rsi)
        return arg1;
    
    telemetry_board::TcpConnection::~TcpConnection();
    *arg1 = *rsi;
    *rsi = 0;
    return arg1;
}

int64_t std::chrono::duration<int64_t, std::ratio<1l, 1000000000l> >::count()
{
    int64_t* rdi;
    return *rdi;
}

int64_t std::to_chars(char* arg1, char* arg2, uint8_t arg3, int32_t arg4)
{
    return std::__to_chars_i<uint8_t>(arg1, arg2, arg3, arg4);
}

int64_t std::to_chars(char* arg1, char* arg2, uint32_t arg3, int32_t arg4)
{
    return std::__to_chars_i<uint32_t>(arg1, arg2, arg3, arg4);
}

int64_t std::to_chars(char* arg1, char* arg2, uint64_t arg3, int32_t arg4)
{
    return std::__to_chars_i<uint64_t>(arg1, arg2, arg3, arg4);
}

int64_t std::to_chars(char* arg1, char* arg2, uint64_t arg3, int32_t arg4)
{
    return std::__to_chars_i<uint64_t>(arg1, arg2, arg3, arg4);
}

int64_t std::to_chars(char* arg1, char* arg2, uint128_t arg3, int32_t arg4)
{
    void var_38;
    return std::__to_chars_i<uint128_t>(arg1, arg2, var_38, arg4);
}

int32_t std::__bit_width<uint32_t>(uint32_t arg1)
{
    int32_t var_c = 0x20;
    return 0x20 - std::__countl_zero<uint32_t>(arg1);
}

int64_t std::string::size()
{
    void* rdi;
    return *(rdi + 8);
}

uint64_t std::__extent_storage<18446744073709551615ul>::__extent_storage(uint64_t arg1)
{
    int64_t rsi;
    *arg1 = rsi;
    return arg1;
}

int64_t std::__extent_storage<18446744073709551615ul>::_M_extent()
{
    int64_t* rdi;
    return *rdi;
}

int64_t std::__unicode::__is_scalar_value(char32_t arg1) __pure
{
    if (arg1 <= 0xd7ff)
        return 1;
    
    if (arg1 > 0xdfff && arg1 <= 0x10ffff)
        return 1;
    
    return 0;
}

int64_t std::__unicode::_Repl::operator()() __pure
{
    int64_t rdi;
    int64_t var_10 = rdi;
    return 0xfffd;
}

uint64_t std::__unicode::__v15_1_0::__field_width(char32_t arg1)
{
    char32_t var_1c = arg1;
    
    if (var_1c < 0x1100)
        return 1;
    
    int64_t rax_4 = (std::upper_bound<char32_t const*, char32_t>(
        &std::__unicode::__v15_1_0::__width_edges, &std::__unicode::__v15_1_0::__gcb_edges, &var_1c) -
        &std::__unicode::__v15_1_0::__width_edges) >> 2;
    uint32_t rax_7 = rax_4 >> 0x3f >> 0x3f;
    return ((rax_4 + rax_7) & 1) - rax_7 + 1;
}

uint64_t std::__unicode::__v15_1_0::__grapheme_cluster_break_property(char32_t arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int32_t var_24 = 0xf;
    void* const var_20 = &std::__unicode::__v15_1_0::__incb_linkers;
    uint32_t var_28 = arg1 << 4 | 0xf;
    uint64_t result = *(std::lower_bound<uint32_t const*, uint32_t>(
        &std::__unicode::__v15_1_0::__gcb_edges, var_20, &var_28) - 4) & 0xf;
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

char32_t* std::__unicode::__v15_1_0::__is_incb_linker(char32_t arg1)
{
    char32_t var_1c = arg1;
    char32_t* result;
    result = 0x404a28 != std::find<char32_t const*, char32_t>(
        &std::__unicode::__v15_1_0::__incb_linkers, 0x404a28, &var_1c);
    return result;
}

uint64_t std::__unicode::__v15_1_0::__incb_property(char32_t arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    uint64_t result;
    
    if (arg1 << 2 >= 0xc02)
    {
        int32_t var_24_1 = 3;
        void* const var_20_1 = 0x405044;
        uint32_t var_28 = arg1 << 2 | 3;
        result = *(std::lower_bound<uint32_t const*, uint32_t>(
            &std::__unicode::__v15_1_0::__incb_edges, var_20_1, &var_28) - 4) & 3;
    }
    else
        result = 0;
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

uint64_t std::__unicode::__v15_1_0::__is_extended_pictographic(char32_t arg1)
{
    char32_t var_1c = arg1;
    
    if (var_1c < 0xa9)
        return 0;
    
    uint64_t result;
    result = (std::upper_bound<char32_t const*, char32_t>(
        &std::__unicode::__v15_1_0::__xpicto_edges, std::__unicode::__v15_1_0::__field_width, 
        &var_1c) - &std::__unicode::__v15_1_0::__xpicto_edges) >> 2 & 1;
    return result;
}

int32_t* std::__unicode::__v15_1_0::_Grapheme_cluster_iterator_base::_M_reset(char32_t arg1, std::__unicode::__v15_1_0::_Gcb_property arg2)
{
    int32_t* result = arg1;
    *result = arg2;
    int32_t rdx;
    result[1] = rdx;
    result[2] = 0;
    *(result + 9) = 0;
    *(result + 0xa) = 0;
    return result;
}

uint64_t std::__unicode::__v15_1_0::_Grapheme_cluster_iterator_base::_M_update_xpicto_seq_state(char32_t arg1, std::__unicode::__v15_1_0::_Gcb_property arg2)
{
    int32_t* result_1 = arg1;
    uint64_t result = result_1[2];
    
    if (result != 3)
    {
        char var_9_1 = 3;
        int32_t rdx;
        
        if (result_1[2] == 1)
        {
            char rax_11;
            
            if (!rdx)
                rax_11 = std::__unicode::__v15_1_0::__is_extended_pictographic(arg2);
            
            char rax_12;
            
            if (rdx || !rax_11)
                rax_12 = 0;
            else
                rax_12 = 1;
            
            if (rax_12)
                var_9_1 = 2;
        }
        else if (rdx != 0xa)
        {
            if (rdx == 4)
                var_9_1 = result_1[2];
        }
        else if (result_1[2] != 2)
        {
            if (std::__unicode::__v15_1_0::__is_extended_pictographic(*result_1))
                var_9_1 = 1;
        }
        else
            var_9_1 = 1;
        result = result_1;
        *(result + 8) = var_9_1;
    }
    
    return result;
}

void* std::__unicode::__v15_1_0::_Grapheme_cluster_iterator_base::_M_update_ri_count(std::__unicode::__v15_1_0::_Gcb_property arg1)
{
    void* result_1 = arg1;
    void* result;
    int32_t rsi;
    
    if (rsi != 0xd)
    {
        result = result_1;
        *(result + 9) = 0;
    }
    else
    {
        result = result_1;
        *(result + 9) = *(result_1 + 9) + 1;
    }
    
    return result;
}

char32_t* std::__unicode::__v15_1_0::_Grapheme_cluster_iterator_base::_M_update_incb_state(char32_t arg1, std::__unicode::__v15_1_0::_Gcb_property arg2)
{
    int32_t rdx;
    int32_t var_18 = rdx;
    char32_t* result = std::__unicode::__v15_1_0::__is_incb_linker(arg2);
    
    if (result)
    {
        result = arg1;
        *(result + 0xa) = 1;
    }
    
    return result;
}

uint8_t std::__from_chars_alnum_to_val<false>(uint8_t arg1)
{
    return *(arg1 + &std::__from_chars_alnum_to_val_table<false>::value);
}

char const* std::format_error::format_error(char const* arg1)
{
    std::runtime_error::runtime_error(arg1);
    *arg1 = &data_411230;
    return arg1;
}

int64_t std::format_error::~format_error()
{
    void*** rdi;
    *rdi = &data_411230;
    return std::runtime_error::~runtime_error();
}

int64_t std::format_error::~format_error()
{
    std::format_error::~format_error();
    void* rdi;
    return operator delete(rdi, 0x10);
}

void std::__throw_format_error(char const* arg1) __noreturn
{
    void* thrown_exception = __cxa_allocate_exception(0x10);
    std::format_error::format_error(thrown_exception);
    __cxa_throw(thrown_exception, &_typeinfo_for_std::format_error, 
        std::format_error::~format_error);
    /* no return */
}

void std::__format::__unmatched_left_brace_in_format_string() __noreturn
{
    std::__throw_format_error("format error: unmatched '{' in f…");
    /* no return */
}

void std::__format::__unmatched_right_brace_in_format_string() __noreturn
{
    std::__throw_format_error("format error: unmatched '}' in f…");
    /* no return */
}

void std::__format::__conflicting_indexing_in_format_string() __noreturn
{
    std::__throw_format_error("format error: conflicting indexi…");
    /* no return */
}

void std::__format::__invalid_arg_id_in_format_string() __noreturn
{
    std::__throw_format_error("format error: invalid arg-id in …");
    /* no return */
}

void std::__format::__failed_to_parse_format_spec() __noreturn
{
    std::__throw_format_error("format error: failed to parse fo…");
    /* no return */
}

int64_t std::__format::__is_digit(char arg1)
{
    int64_t result;
    result = std::__from_chars_alnum_to_val<false>(arg1) <= 9;
    return result;
}

int64_t std::__format::__is_xdigit(char arg1)
{
    int64_t result;
    result = std::__from_chars_alnum_to_val<false>(arg1) <= 0xf;
    return result;
}

std::locale const& std::__format::_Optional_locale::_Optional_locale(std::locale const& arg1)
{
    std::locale::locale(arg1);
    *(arg1 + 8) = 1;
    return arg1;
}

int64_t std::__format::_Optional_locale::~_Optional_locale()
{
    void* rdi;
    char result = *(rdi + 8);
    
    if (!result)
        return result;
    
    return std::locale::~locale();
}

void* std::__format::_Optional_locale::value()
{
    void* result;
    
    if (*(result + 8) != 1)
    {
        decltype(::new((void*)(0)) std::locale()) std::construct_at<std::locale>(std::locale*)(
            result);
        *(result + 8) = 1;
    }
    
    return result;
}

int64_t std::__format::_Seq_sink<std::string>::~_Seq_sink()
{
    void*** rdi;
    *rdi = &data_4111a0;
    return std::string::~string();
}

std::cxx11::string std::vformat(std::basic_string_view<char> arg1, std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char> > arg2)
{
    void* fsbase;
    int64_t rax_4 = *(fsbase + 0x28);
    void** var_168;
    int64_t* var_198 = &var_168;
    std::__format::_Sink<char>* var_178;
    std::span<char, 18446744073709551615ul>::span<256ul>(&var_178);
    std::__format::_Sink<char>* rax_6 = var_178;
    *var_198 = &data_411200;
    var_198[1] = rax_6;
    int64_t var_170;
    var_198[2] = var_170;
    var_198[3] = std::span<char, 18446744073709551615ul>::begin();
    *var_198 = &data_4111d0;
    var_168 = &data_4111a0;
    std::string::string();
    var_178 = std::addressof<std::__format::_Sink<char> >(&var_168);
    std::__format::_Sink<char>* var_1a8 = var_178;
    std::__format::_Sink<char>* rcx;
    var_178 = rcx;
    int64_t r8;
    int64_t var_170_1 = r8;
    std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char> >* rdx;
    std::__format::__do_vformat_to<std::__format::_Sink_iter<char>, char, std::basic_format_context<std::__format::_Sink_iter<char>, char> >(
        *std::move<std::__format::_Sink_iter<char>&>(&var_1a8), arg2, rdx, &var_178);
    std::__format::_Seq_sink<std::string>::get();
    std::__format::_Seq_sink<std::string>::~_Seq_sink();
    *(fsbase + 0x28);
    
    if (rax_4 == *(fsbase + 0x28))
        return arg1;
    
    __stack_chk_fail();
    /* no return */
}

void sub_405cab(void* arg1 @ rbp) __noreturn
{
    std::__format::_Seq_sink<std::string>::~_Seq_sink();
    void* fsbase;
    
    if (*(arg1 - 0x18) != *(fsbase + 0x28))
    {
        __stack_chk_fail();
        /* no return */
    }
    
    _Unwind_Resume();
    __stack_chk_fail();
    /* no return */
}

int64_t std::span<char, 18446744073709551615ul>::data()
{
    int64_t* rdi;
    return *rdi;
}

int64_t std::span<char, 18446744073709551615ul>::size()
{
    return std::__extent_storage<18446744073709551615ul>::_M_extent();
}

int64_t std::chrono::duration<int64_t, std::ratio<1l, 1000000l> >::count()
{
    int64_t* rdi;
    return *rdi;
}

int64_t Timestamp::now()
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_20 = std::chrono::_V2::system_clock::now();
    int64_t var_18 = std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::duration<int64_t, std::ratio<1l, 1000000000l> > >::time_since_epoch();
    int64_t var_28 = std::chrono::duration_cast<std::chrono::duration<int64_t, std::ratio<1l, 1000000l> >, int64_t, std::ratio<1l, 1000000000l> >(
        &var_18);
    std::chrono::duration<int64_t, std::ratio<1l, 1000000l> >::count();
    int64_t result;
    Timestamp::Timestamp(result);
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

int32_t* Timestamp::Timestamp(Timestamp const& arg1)
{
    int32_t* rsi;
    *rsi;
    return Timestamp::Timestamp(arg1);
}

int32_t* Timestamp::Timestamp(Timestamp&& arg1)
{
    int32_t* rsi;
    *rsi;
    return Timestamp::Timestamp(arg1);
}

Timestamp const& Timestamp::operator=(Timestamp const& arg1)
{
    int32_t* rsi;
    *arg1 = *rsi;
    return arg1;
}

Timestamp&& Timestamp::operator=(Timestamp&& arg1)
{
    int32_t* rsi;
    *arg1 = *rsi;
    return arg1;
}

uint64_t Timestamp::operator uint32_t()
{
    int32_t* rdi;
    return *rdi;
}

int32_t* Timestamp::Timestamp(uint32_t arg1)
{
    int32_t* result = arg1;
    int32_t rsi;
    *result = rsi;
    return result;
}

uint64_t Duration::as_ms()
{
    int32_t* rdi;
    return *rdi / 0x3e8;
}

int32_t* Duration::Duration(uint32_t arg1)
{
    int32_t* result = arg1;
    int32_t rsi;
    *result = rsi;
    return result;
}

uint64_t operator-(Timestamp const& arg1, Timestamp const& arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    Timestamp::operator uint32_t();
    Timestamp::operator uint32_t();
    int32_t var_24;
    Duration::Duration(&var_24);
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return var_24;
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::vprint_nonunicode(_IO_FILE* arg1, std::basic_string_view<char> arg2, std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char> > arg3)
{
    void* fsbase;
    int64_t rax_4 = *(fsbase + 0x28);
    void** var_168;
    int64_t* var_1a8 = &var_168;
    std::__format::_Sink<char>* var_178;
    std::span<char, 18446744073709551615ul>::span<256ul>(&var_178);
    std::__format::_Sink<char>* rax_6 = var_178;
    *var_1a8 = &data_411200;
    var_1a8[1] = rax_6;
    int64_t var_170;
    var_1a8[2] = var_170;
    var_1a8[3] = std::span<char, 18446744073709551615ul>::begin();
    *var_1a8 = &data_4111d0;
    var_168 = &data_4111a0;
    std::string::string();
    var_178 = std::addressof<std::__format::_Sink<char> >(&var_168);
    std::__format::_Sink<char>* var_1b8 = var_178;
    std::__format::_Sink<char>* rcx;
    var_178 = rcx;
    int64_t r8;
    int64_t var_170_1 = r8;
    std::__format::__do_vformat_to<std::__format::_Sink_iter<char>, char, std::basic_format_context<std::__format::_Sink_iter<char>, char> >(
        *std::move<std::__format::_Sink_iter<char>&>(&var_1b8), arg2, arg3, &var_178);
    char* rax_18;
    int64_t rdx_8;
    rax_18 = std::__format::_Seq_sink<std::string>::view();
    char* var_198 = rax_18;
    int64_t var_190 = rdx_8;
    int64_t rax_19 = std::span<char, 18446744073709551615ul>::size();
    int64_t rax_23;
    rax_23 = fwrite(std::span<char, 18446744073709551615ul>::data(), 1, rax_19, arg1)
        != std::span<char, 18446744073709551615ul>::size();
    
    if (!rax_23)
    {
        std::__format::_Seq_sink<std::string>::~_Seq_sink();
        
        if (rax_4 == *(fsbase + 0x28))
            return rax_4 - *(fsbase + 0x28);
        
        __stack_chk_fail();
        /* no return */
    }
    
    *(fsbase + 0x28);
    
    if (rax_4 != *(fsbase + 0x28))
    {
        __stack_chk_fail();
        /* no return */
    }
    
    std::__throw_system_error(5);
    /* no return */
}

void sub_4061e7(void* arg1 @ rbp) __noreturn
{
    std::__format::_Seq_sink<std::string>::~_Seq_sink();
    void* fsbase;
    
    if (*(arg1 - 0x18) != *(fsbase + 0x28))
    {
        __stack_chk_fail();
        /* no return */
    }
    
    _Unwind_Resume();
    __stack_chk_fail();
    /* no return */
}

int64_t std::vprint_unicode(_IO_FILE* arg1, std::basic_string_view<char> arg2, std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char> > arg3)
{
    int64_t rcx;
    int64_t var_38 = rcx;
    return std::vprint_nonunicode(arg1, arg2, arg3);
}

char const* std::basic_string_view<char>::basic_string_view(char const* arg1)
{
    char* rsi;
    *arg1 = std::char_traits<char>::length(rsi);
    *(arg1 + 8) = rsi;
    return arg1;
}

int64_t char_traits<char>::length(char const* arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t result = 0;
    
    while (true)
    {
        char var_19 = 0;
        
        if (char_traits<char>::eq(&arg1[result], &var_19) == 1)
            break;
        
        result += 1;
    }
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

uint64_t char_traits<char>::eq(char const& arg1, char const& arg2)
{
    uint64_t result;
    result = *arg1 == *arg2;
    return result;
}

void* canzero::telemetry::CanzeroConnection::CanzeroConnection()
{
    telemetry_board::TcpConnection::TcpConnection();
    void* result;
    *(result + 8) = 0;
    *(result + 0xc) = 0;
    return result;
}

int64_t* std::__format::_Scanner<char>::_Scanner(std::basic_string_view<char> arg1, uint64_t arg2)
{
    void*** var_10 = arg1;
    *var_10 = &data_411180;
    return std::basic_format_parse_context<char>::basic_format_parse_context(&var_10[1], arg2);
}

int64_t* std::basic_format_parse_context<char>::basic_format_parse_context(std::basic_string_view<char> arg1, uint64_t arg2)
{
    int64_t* result = arg1;
    uint64_t var_28 = arg2;
    int64_t rdx;
    int64_t var_20 = rdx;
    *result = std::basic_string_view<char>::begin();
    result[1] = std::basic_string_view<char>::end();
    result[2] = 0;
    result[3] = 0;
    int64_t rcx;
    result[4] = rcx;
    return result;
}

int64_t std::basic_string_view<char>::begin()
{
    void* rdi;
    return *(rdi + 8);
}

int64_t std::basic_string_view<char>::end()
{
    int64_t* rdi;
    return *rdi + rdi[1];
}

int64_t std::basic_string_view<char>::operator[](uint64_t arg1)
{
    int64_t rsi;
    return rsi + *(arg1 + 8);
}

int64_t std::__format::_Scanner<char>::_M_scan()
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t rax_2;
    int64_t rdx;
    rax_2 = std::__format::_Scanner<char>::_M_fmt_str();
    int64_t var_38 = rax_2;
    int64_t var_30 = rdx;
    char rax_8;
    
    if (std::basic_string_view<char>::size() != 2)
        rax_8 = 0;
    else if (*std::basic_string_view<char>::operator[](&var_38) != 0x7b)
        rax_8 = 0;
    else if (*std::basic_string_view<char>::operator[](&var_38) != 0x7d)
        rax_8 = 0;
    else
        rax_8 = 1;
    
    int64_t* rdi;
    
    if (!rax_8)
    {
        void* var_58_1 = std::basic_string_view<char>::find(&var_38, 0x7b);
        void* rax_21;
        int64_t rsi_5;
        rax_21 = std::basic_string_view<char>::find(&var_38, 0x7d);
        void* var_50_1 = rax_21;
        
        while (true)
        {
            int64_t rax_85;
            rax_85 = std::basic_string_view<char>::size();
            
            if (!rax_85)
                break;
            
            char var_5a_1;
            
            if (var_58_1 == var_50_1)
                var_5a_1 = 0;
            else if (var_58_1 >= var_50_1)
                var_5a_1 = 1;
            else
                var_5a_1 = 0xff;
            
            if (std::operator==(var_5a_1, rsi_5))
            {
                (**rdi)(rdi, std::__format::_Scanner<char>::end());
                std::__format::_Scanner<char>::end();
                std::basic_format_parse_context<char>::advance_to(&rdi[1]);
                break;
            }
            
            if (!std::operator<(var_5a_1, rsi_5))
            {
                char rax_68;
                
                if (var_50_1 + 1 == std::basic_string_view<char>::size())
                    rax_68 = 1;
                else if (*std::basic_string_view<char>::operator[](&var_38) == 0x7d)
                    rax_68 = 0;
                else
                    rax_68 = 1;
                
                if (rax_68)
                {
                    *(fsbase + 0x28);
                    
                    if (rax != *(fsbase + 0x28))
                    {
                        __stack_chk_fail();
                        /* no return */
                    }
                    
                    std::__format::__unmatched_right_brace_in_format_string();
                    /* no return */
                }
                
                (**rdi)(rdi, std::__format::_Scanner<char>::begin() + var_50_1 + 1);
                std::basic_format_parse_context<char>::advance_to(&rdi[1]);
                int64_t rax_80;
                int64_t rdx_23;
                rax_80 = std::__format::_Scanner<char>::_M_fmt_str();
                var_38 = rax_80;
                int64_t var_30_3 = rdx_23;
                
                if (var_58_1 != -1)
                    var_58_1 = var_58_1 - (var_50_1 + 1) - 1;
                
                void* rax_84;
                rax_84 = std::basic_string_view<char>::find(&var_38, 0x7d);
                var_50_1 = rax_84;
            }
            else
            {
                char rax_40;
                
                if (var_58_1 + 1 == std::basic_string_view<char>::size())
                    rax_40 = 1;
                else if (var_50_1 != -1)
                    rax_40 = 0;
                else if (*std::basic_string_view<char>::operator[](&var_38) == 0x7b)
                    rax_40 = 0;
                else
                    rax_40 = 1;
                
                if (rax_40)
                {
                    *(fsbase + 0x28);
                    
                    if (rax != *(fsbase + 0x28))
                    {
                        __stack_chk_fail();
                        /* no return */
                    }
                    
                    std::__format::__unmatched_left_brace_in_format_string();
                    /* no return */
                }
                
                bool rax_45 = *std::basic_string_view<char>::operator[](&var_38) == 0x7b;
                (**rdi)(rdi, std::__format::_Scanner<char>::begin() + var_58_1 + rax_45);
                std::basic_format_parse_context<char>::advance_to(&rdi[1]);
                int64_t rax_55;
                int64_t rdx_13;
                rax_55 = std::__format::_Scanner<char>::_M_fmt_str();
                var_38 = rax_55;
                int64_t var_30_1 = rdx_13;
                
                if (!rax_45)
                {
                    std::__format::_Scanner<char>::_M_on_replacement_field();
                    int64_t rax_62;
                    int64_t rdx_15;
                    rax_62 = std::__format::_Scanner<char>::_M_fmt_str();
                    var_38 = rax_62;
                    int64_t var_30_2 = rdx_15;
                    var_58_1 = std::basic_string_view<char>::find(&var_38, 0x7b);
                    void* rax_64;
                    rax_64 = std::basic_string_view<char>::find(&var_38, 0x7d);
                    var_50_1 = rax_64;
                }
                else
                {
                    if (var_50_1 != -1)
                        var_50_1 = var_50_1 - var_58_1 - 2;
                    
                    void* rax_59;
                    rax_59 = std::basic_string_view<char>::find(&var_38, 0x7b);
                    var_58_1 = rax_59;
                }
            }
        }
    }
    else
    {
        std::__format::_Scanner<char>::begin();
        std::basic_format_parse_context<char>::advance_to(&rdi[1]);
        (*(*rdi + 8))(rdi, std::basic_format_parse_context<char>::next_arg_id());
    }
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::__format::_Scanner<char>::_M_fmt_str()
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    char* rax_2 = std::__format::_Scanner<char>::begin();
    std::__format::_Scanner<char>::end();
    int64_t result;
    std::basic_string_view<char>::basic_string_view<char const*, char const*>(&result, rax_2);
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::__format::_Scanner<char>::begin()
{
    return std::basic_format_parse_context<char>::begin();
}

int64_t std::basic_format_parse_context<char>::begin()
{
    int64_t* rdi;
    return *rdi;
}

int64_t std::__format::_Scanner<char>::end()
{
    return std::basic_format_parse_context<char>::end();
}

int64_t std::basic_format_parse_context<char>::end()
{
    void* rdi;
    return *(rdi + 8);
}

char* std::basic_string_view<char>::basic_string_view<char const*, char const*>(char const* arg1, char const* arg2)
{
    int64_t rdx;
    *arg1 = rdx - arg2;
    char* result = std::to_address<char const>(arg2);
    *(arg1 + 8) = result;
    return result;
}

char* std::to_address<char const>(char const* arg1)
{
    return std::__to_address<char const>(arg1);
}

char const* std::__to_address<char const>(char const* arg1) __pure
{
    return arg1;
}

void* std::basic_string_view<char>::find(char arg1, uint64_t arg2)
{
    int64_t* var_30 = arg1;
    char var_34 = arg2;
    void* result = -ffffffffffffffff;
    int64_t rdx;
    
    if (rdx < *var_30)
    {
        void* rax_9 = std::char_traits<char>::find(var_30[1] + rdx, *var_30 - rdx, &var_34);
        
        if (rax_9)
            return rax_9 - var_30[1];
    }
    
    return result;
}

void* char_traits<char>::find(char const* arg1, uint64_t arg2, char const& arg3)
{
    void* var_10 = nullptr;
    
    while (true)
    {
        if (var_10 >= arg2)
            return nullptr;
        
        if (char_traits<char>::eq(arg1 + var_10, arg3))
            break;
        
        var_10 += 1;
    }
    
    return var_10 + arg1;
}

int64_t std::__format::_Scanner<char>::_M_on_chars(char const* arg1) __pure
{
    char const* var_10 = arg1;
    int64_t rsi;
    int64_t var_18 = rsi;
}

char const* std::basic_format_parse_context<char>::advance_to(char const* arg1)
{
    int64_t rsi;
    *arg1 = rsi;
    return arg1;
}

int64_t canzero::telemetry::CanzeroConnection::close()
{
    telemetry_board::TcpConnection::close();
    void* rdi;
    *(rdi + 8) = 0;
    return std::println<>(0x11, "Closed connection");
}

std::remove_reference<uint16_t>::type* std::get<0ul, uint16_t, char const*>(std::pair<uint16_t, char const*>&& arg1)
{
    return std::__pair_get<0ul>::__move_get<uint16_t, char const*>(
        std::move<std::pair<uint16_t, char const*>&>(arg1));
}

std::pair<uint16_t, char const*>&&& std::move<std::pair<uint16_t, char const*>&>(std::pair<uint16_t, char const*>&&& arg1) __pure
{
    return arg1;
}

std::remove_reference<uint16_t>::type* std::__pair_get<0ul>::__move_get<uint16_t, char const*>(std::pair<uint16_t, char const*>&& arg1)
{
    return std::forward<uint16_t>(arg1);
}

std::remove_reference<uint16_t>::type& std::forward<uint16_t>(std::remove_reference<uint16_t>::type& arg1) __pure
{
    return arg1;
}

std::remove_reference<char const*>::type* std::get<1ul, uint16_t, char const*>(std::pair<uint16_t, char const*>&& arg1)
{
    return std::__pair_get<1ul>::__move_get<uint16_t, char const*>(
        std::move<std::pair<uint16_t, char const*>&>(arg1));
}

std::remove_reference<char const*>::type* std::__pair_get<1ul>::__move_get<uint16_t, char const*>(std::pair<uint16_t, char const*>&& arg1)
{
    return std::forward<char const*>(arg1 + 8);
}

std::remove_reference<char const*>::type& std::forward<char const*>(std::remove_reference<char const*>::type& arg1) __pure
{
    return arg1;
}

int64_t std::__format::_Scanner<char>::_M_on_replacement_field()
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    char* rax_2 = std::__format::_Scanner<char>::begin();
    uint64_t var_58;
    int64_t* rdi;
    
    if (*rax_2 == 0x7d)
        var_58 = std::basic_format_parse_context<char>::next_arg_id();
    else if (*rax_2 != 0x3a)
    {
        char* rax_16 = std::__format::_Scanner<char>::end();
        int32_t rax_19;
        int64_t rdx_2;
        rax_19 =
            std::__format::__parse_arg_id<char>(std::__format::_Scanner<char>::begin(), rax_16);
        int32_t var_38 = rax_19;
        int64_t var_30_1 = rdx_2;
        std::remove_reference<uint16_t>::type* rax_21 =
            std::get<0ul, uint16_t, char const*>(&var_38);
        std::remove_reference<char const*>::type* rax_22 =
            std::get<1ul, uint16_t, char const*>(&var_38);
        
        if (!*rax_22 || (**rax_22 != 0x7d && **rax_22 != 0x3a))
        {
            *(fsbase + 0x28);
            
            if (rax != *(fsbase + 0x28))
            {
                __stack_chk_fail();
                /* no return */
            }
            
            std::__format::__invalid_arg_id_in_format_string();
            /* no return */
        }
        
        var_58 = *rax_21;
        std::basic_format_parse_context<char>::check_arg_id(&rdi[1]);
        
        if (**rax_22 != 0x3a)
        {
            *rax_22;
            std::basic_format_parse_context<char>::advance_to(&rdi[1]);
        }
        else
        {
            *rax_22 += 1;
            *rax_22;
            std::basic_format_parse_context<char>::advance_to(&rdi[1]);
        }
    }
    else
    {
        var_58 = std::basic_format_parse_context<char>::next_arg_id();
        std::basic_format_parse_context<char>::advance_to(&rdi[1]);
    }
    
    (*(*rdi + 8))(rdi, var_58);
    int64_t rax_55 = std::__format::_Scanner<char>::begin();
    int64_t rax_57 = std::__format::_Scanner<char>::end();
    char rax_60;
    
    if (rax_55 != rax_57)
        rax_60 = *std::__format::_Scanner<char>::begin();
    
    char rax_61;
    
    if (rax_55 != rax_57 && rax_60 == 0x7d)
        rax_61 = 0;
    else
        rax_61 = 1;
    
    if (!rax_61)
    {
        std::__format::_Scanner<char>::begin();
        std::basic_format_parse_context<char>::advance_to(&rdi[1]);
        
        if (rax == *(fsbase + 0x28))
            return rax - *(fsbase + 0x28);
        
        __stack_chk_fail();
        /* no return */
    }
    
    *(fsbase + 0x28);
    
    if (rax != *(fsbase + 0x28))
    {
        __stack_chk_fail();
        /* no return */
    }
    
    std::__format::__unmatched_left_brace_in_format_string();
    /* no return */
}

int64_t std::basic_format_parse_context<char>::next_arg_id()
{
    void* rdi;
    
    if (*(rdi + 0x10) == 1)
    {
        std::__format::__conflicting_indexing_in_format_string();
        /* no return */
    }
    
    *(rdi + 0x10) = 2;
    int64_t result = *(rdi + 0x18);
    *(rdi + 0x18) = result + 1;
    return result;
}

int64_t* std::__format::__formatter_int<char>::_M_do_parse(std::basic_format_parse_context<char>&, std::__format::_Pres_type)::'lambda'()::operator()()
{
    int64_t* rdi;
    int64_t* rdx = rdi[1];
    int64_t* result = *rdi;
    *result = *rdx;
    result[1] = rdx[1];
    return result;
}

int64_t std::__format::__formatter_int<char>::_M_do_parse(std::basic_format_parse_context<char>&, std::__format::_Pres_type)::'lambda0'()::operator()()
{
    int64_t* rdi;
    
    if (**rdi != *rdi[1] && ***rdi != 0x7d)
        return 0;
    
    rdi[2];
    std::__format::__formatter_int<char>::_M_do_parse(std::basic_format_parse_context<char>&, std::__format::_Pres_type)::'lambda'()::operator()();
    return 1;
}

void* std::__format::__formatter_int<char>::_M_do_parse(std::basic_format_parse_context<char>& arg1, std::__format::_Pres_type arg2)
{
    int64_t var_68 = arg2;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_44 = 0;
    int32_t var_3c = 0;
    int32_t var_3c_1 = 0x20;
    int32_t rdx;
    *var_44[1] = (*var_44[1] & 0x87) | (rdx & 0xf) << 3;
    std::basic_format_parse_context<char>* var_58 = std::basic_format_parse_context<char>::end();
    void* result_1 = std::basic_format_parse_context<char>::begin();
    std::basic_format_parse_context<char>& var_38 = arg1;
    int64_t* var_30 = &var_44;
    int64_t* var_28 = &result_1;
    int64_t* var_20 = &var_58;
    std::basic_format_parse_context<char>&* var_18 = &var_38;
    void* result;
    
    if (!std::__format::__formatter_int<char>::_M_do_parse(std::basic_format_parse_context<char>&, std::__format::_Pres_type)::'lambda0'()::operator()())
    {
        result_1 = std::__format::_Spec<char>::_M_parse_fill_and_align(&var_44, result_1);
        
        if (!std::__format::__formatter_int<char>::_M_do_parse(std::basic_format_parse_context<char>&, std::__format::_Pres_type)::'lambda0'()::operator()())
        {
            result_1 = std::__format::_Spec<char>::_M_parse_sign(&var_44, result_1);
            
            if (!std::__format::__formatter_int<char>::_M_do_parse(std::basic_format_parse_context<char>&, std::__format::_Pres_type)::'lambda0'()::operator()())
            {
                result_1 = std::__format::_Spec<char>::_M_parse_alternate_form(&var_44, result_1);
                
                if (!std::__format::__formatter_int<char>::_M_do_parse(std::basic_format_parse_context<char>&, std::__format::_Pres_type)::'lambda0'()::operator()())
                {
                    result_1 = std::__format::_Spec<char>::_M_parse_zero_fill(&var_44, result_1);
                    
                    if (!std::__format::__formatter_int<char>::_M_do_parse(std::basic_format_parse_context<char>&, std::__format::_Pres_type)::'lambda0'()::operator()())
                    {
                        result_1 =
                            std::__format::_Spec<char>::_M_parse_width(&var_44, result_1, var_58);
                        
                        if (!std::__format::__formatter_int<char>::_M_do_parse(std::basic_format_parse_context<char>&, std::__format::_Pres_type)::'lambda0'()::operator()())
                        {
                            result_1 =
                                std::__format::_Spec<char>::_M_parse_locale(&var_44, result_1);
                            
                            if (!std::__format::__formatter_int<char>::_M_do_parse(std::basic_format_parse_context<char>&, std::__format::_Pres_type)::'lambda0'()::operator()())
                            {
                                switch (*result_1 - 0x42)
                                {
                                    case 0:
                                    {
                                        *var_44[1] = (*var_44[1] & 0x87) | 0x18;
                                        result_1 += 1;
                                        break;
                                    }
                                    case 0x16:
                                    {
                                        *var_44[1] = (*var_44[1] & 0x87) | 0x30;
                                        result_1 += 1;
                                        break;
                                    }
                                    case 0x20:
                                    {
                                        *var_44[1] = (*var_44[1] & 0x87) | 0x10;
                                        result_1 += 1;
                                        break;
                                    }
                                    case 0x21:
                                    {
                                        if (rdx)
                                        {
                                            *var_44[1] = (*var_44[1] & 0x87) | 0x38;
                                            result_1 += 1;
                                        }
                                        break;
                                    }
                                    case 0x22:
                                    {
                                        *var_44[1] = (*var_44[1] & 0x87) | 8;
                                        result_1 += 1;
                                        break;
                                    }
                                    case 0x2d:
                                    {
                                        *var_44[1] = (*var_44[1] & 0x87) | 0x20;
                                        result_1 += 1;
                                        break;
                                    }
                                    case 0x31:
                                    {
                                        if (!rdx)
                                        {
                                            *var_44[1] &= 0x87;
                                            result_1 += 1;
                                        }
                                        break;
                                    }
                                    case 0x36:
                                    {
                                        *var_44[1] = (*var_44[1] & 0x87) | 0x28;
                                        result_1 += 1;
                                        break;
                                    }
                                }
                                
                                if (!std::__format::__formatter_int<char>::_M_do_parse(std::basic_format_parse_context<char>&, std::__format::_Pres_type)::'lambda0'()::operator()())
                                {
                                    *(fsbase + 0x28);
                                    
                                    if (rax != *(fsbase + 0x28))
                                    {
                                        __stack_chk_fail();
                                        /* no return */
                                    }
                                    
                                    std::__format::__failed_to_parse_format_spec();
                                    /* no return */
                                }
                                
                                result = result_1;
                            }
                            else
                                result = result_1;
                        }
                        else
                            result = result_1;
                    }
                    else
                        result = result_1;
                }
                else
                    result = result_1;
            }
            else
                result = result_1;
        }
        else
            result = result_1;
    }
    else
        result = result_1;
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::_Optional_base<telemetry_board::TcpConnection, false, false>::~_Optional_base()
{
    return std::_Optional_payload<telemetry_board::TcpConnection, false, false, false>::~_Optional_payload();
}

int64_t std::optional<telemetry_board::TcpConnection>::~optional()
{
    return std::_Optional_base<telemetry_board::TcpConnection, false, false>::~_Optional_base();
}

void std::_Optional_payload_base<Timestamped<canzero_frame> >::_Storage<Timestamped<canzero_frame>, true>::_Storage() __pure
{
    int64_t rdi;
    int64_t var_10 = rdi;
}

void* std::_Optional_payload_base<Timestamped<canzero_frame> >::_Optional_payload_base()
{
    void* result;
    *(result + 0x14) = 0;
    return result;
}

void* std::_Optional_payload<Timestamped<canzero_frame>, true, false, false>::_Optional_payload()
{
    return std::_Optional_payload_base<Timestamped<canzero_frame> >::_Optional_payload_base();
}

void* std::_Optional_base<Timestamped<canzero_frame>, false, false>::_Optional_base()
{
    return 
        std::_Optional_payload<Timestamped<canzero_frame>, true, false, false>::_Optional_payload();
}

std::_Optional_base<Timestamped<canzero_frame>, false, false>&& std::_Optional_base<Timestamped<canzero_frame>, false, false>::operator=(std::_Optional_base<Timestamped<canzero_frame>, false, false>&& arg1)
{
    std::_Optional_payload<Timestamped<canzero_frame>, true, false, false>::operator=(arg1);
    return arg1;
}

std::optional<Timestamped<canzero_frame> >&& std::optional<Timestamped<canzero_frame> >::operator=(std::optional<Timestamped<canzero_frame> >&& arg1)
{
    std::_Optional_base<Timestamped<canzero_frame>, false, false>::operator=(arg1);
    return arg1;
}

void* decltype(::new((void*)(0)) char(declval<char const&>())) std::construct_at<char, char const&>(char*, char const&)(void* arg1, std::remove_reference<char const&>::type* arg2)
{
    void* result = operator new(1, arg1);
    *result = *std::forward<char const&>(arg2);
    return result;
}

char* char_traits<char>::move(char* arg1, char const* arg2, uint64_t arg3)
{
    if (!arg3)
        return arg1;
    
    memmove(arg1, arg2, arg3);
    return arg1;
}

char* char_traits<char>::copy(char* arg1, char const* arg2, uint64_t arg3)
{
    if (!arg3)
        return arg1;
    
    memcpy(arg1, arg2, arg3, arg1);
    return arg1;
}

char* char_traits<char>::assign(char* arg1, uint64_t arg2, char arg3)
{
    char var_3c = arg3;
    void* fsbase;
    int64_t rax_1 = *(fsbase + 0x28);
    
    if (arg2)
        memset(arg1, var_3c, arg2);
    
    *(fsbase + 0x28);
    
    if (rax_1 == *(fsbase + 0x28))
        return arg1;
    
    __stack_chk_fail();
    /* no return */
}

char const* std::basic_string_view<char>::basic_string_view(char const* arg1, uint64_t arg2)
{
    int64_t rdx;
    *arg1 = rdx;
    *(arg1 + 8) = arg2;
    return arg1;
}

int64_t std::string::_M_data()
{
    int64_t* rdi;
    return *rdi;
}

int64_t std::string::_M_set_length(uint64_t arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    std::string::_M_length(arg1);
    char var_11 = 0;
    void* rsi;
    std::char_traits<char>::assign(rsi + std::string::_M_data(), &var_11);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::string::~string()
{
    std::string::_M_dispose();
    return std::string::_Alloc_hider::~_Alloc_hider();
}

int64_t std::__to_chars_10_impl<uint32_t>(char* arg1, uint32_t arg2, uint32_t arg3)
{
    uint32_t i = arg3;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_e8;
    __builtin_strcpy(&var_e8, 
        "00010203040506070809101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899");
    int32_t var_f4 = arg2 - 1;
    
    while (i > 0x63)
    {
        int32_t rax_9 = i % 0x64 * 2;
        i u/= 0x64;
        arg1[var_f4] = *(&var_e8 + rax_9 + 1);
        arg1[var_f4 - 1] = *(&var_e8 + rax_9);
        var_f4 -= 2;
    }
    
    if (i <= 9)
        *arg1 = i + 0x30;
    else
    {
        uint32_t rax_25 = i * 2;
        arg1[1] = *(&var_e8 + rax_25 + 1);
        *arg1 = *(&var_e8 + rax_25);
    }
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::string::_M_get_allocator() __pure
{
    int64_t result;
    return result;
}

int64_t std::string::_M_is_local()
{
    char* rax_3;
    rax_3 = std::string::_M_data() == std::string::_M_local_data();
    
    if (!rax_3)
        return 0;
    
    return 1;
}

int64_t std::string::length()
{
    void* rdi;
    return *(rdi + 8);
}

int64_t std::string::string(std::string&& arg1)
{
    std::string::_Alloc_hider::_Alloc_hider(arg1, std::string::_M_local_data());
    uint64_t rsi;
    
    if (!std::string::_M_is_local())
    {
        std::string::_M_data();
        std::string::_M_data(arg1);
        *(rsi + 0x10);
        std::string::_M_capacity(arg1);
    }
    else
        std::char_traits<char>::copy(arg1 + 0x10, rsi + 0x10, std::string::length() + 1);
    
    std::string::length();
    std::string::_M_length(arg1);
    std::string::_M_local_data();
    std::string::_M_data(rsi);
    return std::string::_M_set_length(rsi);
}

int64_t std::__to_chars_10_impl<uint64_t>(char* arg1, uint32_t arg2, uint64_t arg3)
{
    uint64_t i = arg3;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_e8;
    __builtin_strcpy(&var_e8, 
        "00010203040506070809101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899");
    int32_t var_fc = arg2 - 1;
    
    while (i > 0x63)
    {
        uint64_t rax_11 = i % 0x64 * 2;
        i u/= 0x64;
        arg1[var_fc] = *(&var_e8 + rax_11 + 1);
        arg1[var_fc - 1] = *(rax_11 + &var_e8);
        var_fc -= 2;
    }
    
    if (i <= 9)
        *arg1 = i + 0x30;
    else
    {
        uint64_t rax_27 = i * 2;
        arg1[1] = *(&var_e8 + rax_27 + 1);
        *arg1 = *(rax_27 + &var_e8);
    }
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::__to_chars_10_impl<uint64_t>(char* arg1, uint32_t arg2, uint64_t arg3)
{
    uint64_t i = arg3;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_e8;
    __builtin_strcpy(&var_e8, 
        "00010203040506070809101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899");
    int32_t var_fc = arg2 - 1;
    
    while (i > 0x63)
    {
        uint64_t rax_11 = i % 0x64 * 2;
        i u/= 0x64;
        arg1[var_fc] = *(&var_e8 + rax_11 + 1);
        arg1[var_fc - 1] = *(rax_11 + &var_e8);
        var_fc -= 2;
    }
    
    if (i <= 9)
        *arg1 = i + 0x30;
    else
    {
        uint64_t rax_27 = i * 2;
        arg1[1] = *(&var_e8 + rax_27 + 1);
        *arg1 = *(rax_27 + &var_e8);
    }
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::operator std::basic_string_view<char, std::char_traits<char> >()
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    std::string::size();
    int64_t result;
    std::basic_string_view<char>::basic_string_view(&result, std::string::data());
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

char* std::string::_Alloc_hider::_Alloc_hider(char* arg1, std::allocator<char> const& arg2)
{
    char* var_18 = arg1;
    int64_t rdx;
    int64_t var_10 = rdx;
    *arg1 = arg2;
    return arg1;
}

int64_t std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::duration<int64_t, std::ratio<1l, 1000000000l> > >::time_since_epoch()
{
    int64_t* rdi;
    return *rdi;
}

std::to_chars_result std::__to_chars_i<uint8_t>(char* arg1, char* arg2, uint8_t arg3, int32_t arg4)
{
    uint32_t rax_1 = arg3;
    char* result;
    
    if (arg1 == arg2)
        result = arg2;
    else if (!arg3)
    {
        *arg1 = 0x30;
        result = &arg1[1];
    }
    else if (arg4 == 0x10)
    {
        char* result_1;
        int64_t rdx_3;
        result_1 = std::__to_chars_16<uint32_t>(arg1, arg2, rax_1);
        result = result_1;
    }
    else
    {
        char* result_5;
        int64_t rdx_11;
        
        if (arg4 > 0x10)
        {
            result_5 = std::__to_chars<uint32_t>(arg1, arg2, rax_1, arg4);
            result = result_5;
        }
        else if (arg4 == 0xa)
        {
            char* result_2;
            int64_t rdx_5;
            result_2 = std::__to_chars_10<uint32_t>(arg1, arg2, rax_1);
            result = result_2;
        }
        else if (arg4 > 0xa)
        {
            result_5 = std::__to_chars<uint32_t>(arg1, arg2, rax_1, arg4);
            result = result_5;
        }
        else if (arg4 == 2)
        {
            char* result_4;
            int64_t rdx_9;
            result_4 = std::__to_chars_2<uint32_t>(arg1, arg2, rax_1);
            result = result_4;
        }
        else if (arg4 == 8)
        {
            char* result_3;
            int64_t rdx_7;
            result_3 = std::__to_chars_8<uint32_t>(arg1, arg2, rax_1);
            result = result_3;
        }
        else
        {
            result_5 = std::__to_chars<uint32_t>(arg1, arg2, rax_1, arg4);
            result = result_5;
        }
    }
    
    return result;
}

std::to_chars_result std::__to_chars_i<uint32_t>(char* arg1, char* arg2, uint32_t arg3, int32_t arg4)
{
    char* result;
    
    if (arg1 == arg2)
        result = arg2;
    else if (!arg3)
    {
        *arg1 = 0x30;
        result = &arg1[1];
    }
    else if (arg4 == 0x10)
    {
        char* result_1;
        int64_t rdx_3;
        result_1 = std::__to_chars_16<uint32_t>(arg1, arg2, arg3);
        result = result_1;
    }
    else
    {
        char* result_5;
        int64_t rdx_11;
        
        if (arg4 > 0x10)
        {
            result_5 = std::__to_chars<uint32_t>(arg1, arg2, arg3, arg4);
            result = result_5;
        }
        else if (arg4 == 0xa)
        {
            char* result_2;
            int64_t rdx_5;
            result_2 = std::__to_chars_10<uint32_t>(arg1, arg2, arg3);
            result = result_2;
        }
        else if (arg4 > 0xa)
        {
            result_5 = std::__to_chars<uint32_t>(arg1, arg2, arg3, arg4);
            result = result_5;
        }
        else if (arg4 == 2)
        {
            char* result_4;
            int64_t rdx_9;
            result_4 = std::__to_chars_2<uint32_t>(arg1, arg2, arg3);
            result = result_4;
        }
        else if (arg4 == 8)
        {
            char* result_3;
            int64_t rdx_7;
            result_3 = std::__to_chars_8<uint32_t>(arg1, arg2, arg3);
            result = result_3;
        }
        else
        {
            result_5 = std::__to_chars<uint32_t>(arg1, arg2, arg3, arg4);
            result = result_5;
        }
    }
    
    return result;
}

std::to_chars_result std::__to_chars_i<uint64_t>(char* arg1, char* arg2, uint64_t arg3, int32_t arg4)
{
    char* result;
    
    if (arg1 == arg2)
        result = arg2;
    else if (!arg3)
    {
        *arg1 = 0x30;
        result = &arg1[1];
    }
    else if (arg4 == 0x10)
    {
        char* result_1;
        int64_t rdx_3;
        result_1 = std::__to_chars_16<uint64_t>(arg1, arg2, arg3);
        result = result_1;
    }
    else
    {
        char* result_5;
        int64_t rdx_11;
        
        if (arg4 > 0x10)
        {
            result_5 = std::__to_chars<uint64_t>(arg1, arg2, arg3, arg4);
            result = result_5;
        }
        else if (arg4 == 0xa)
        {
            char* result_2;
            int64_t rdx_5;
            result_2 = std::__to_chars_10<uint64_t>(arg1, arg2, arg3);
            result = result_2;
        }
        else if (arg4 > 0xa)
        {
            result_5 = std::__to_chars<uint64_t>(arg1, arg2, arg3, arg4);
            result = result_5;
        }
        else if (arg4 == 2)
        {
            char* result_4;
            int64_t rdx_9;
            result_4 = std::__to_chars_2<uint64_t>(arg1, arg2, arg3);
            result = result_4;
        }
        else if (arg4 == 8)
        {
            char* result_3;
            int64_t rdx_7;
            result_3 = std::__to_chars_8<uint64_t>(arg1, arg2, arg3);
            result = result_3;
        }
        else
        {
            result_5 = std::__to_chars<uint64_t>(arg1, arg2, arg3, arg4);
            result = result_5;
        }
    }
    
    return result;
}

std::to_chars_result std::__to_chars_i<uint64_t>(char* arg1, char* arg2, uint64_t arg3, int32_t arg4)
{
    char* result;
    
    if (arg1 == arg2)
        result = arg2;
    else if (!arg3)
    {
        *arg1 = 0x30;
        result = &arg1[1];
    }
    else if (arg4 == 0x10)
    {
        char* result_1;
        int64_t rdx_3;
        result_1 = std::__to_chars_16<uint64_t>(arg1, arg2, arg3);
        result = result_1;
    }
    else
    {
        char* result_5;
        int64_t rdx_11;
        
        if (arg4 > 0x10)
        {
            result_5 = std::__to_chars<uint64_t>(arg1, arg2, arg3, arg4);
            result = result_5;
        }
        else if (arg4 == 0xa)
        {
            char* result_2;
            int64_t rdx_5;
            result_2 = std::__to_chars_10<uint64_t>(arg1, arg2, arg3);
            result = result_2;
        }
        else if (arg4 > 0xa)
        {
            result_5 = std::__to_chars<uint64_t>(arg1, arg2, arg3, arg4);
            result = result_5;
        }
        else if (arg4 == 2)
        {
            char* result_4;
            int64_t rdx_9;
            result_4 = std::__to_chars_2<uint64_t>(arg1, arg2, arg3);
            result = result_4;
        }
        else if (arg4 == 8)
        {
            char* result_3;
            int64_t rdx_7;
            result_3 = std::__to_chars_8<uint64_t>(arg1, arg2, arg3);
            result = result_3;
        }
        else
        {
            result_5 = std::__to_chars<uint64_t>(arg1, arg2, arg3, arg4);
            result = result_5;
        }
    }
    
    return result;
}

std::to_chars_result std::__to_chars_i<uint128_t>(char* arg1, char* arg2, uint128_t arg3, int32_t arg4)
{
    int64_t var_38 = arg4;
    char* result;
    int64_t rcx;
    
    if (arg1 == arg2)
        result = arg2;
    else
    {
        int32_t r8;
        
        if (!(var_38 | rcx))
        {
            *arg1 = 0x30;
            result = &arg1[1];
        }
        else
        {
            void var_48;
            
            if (r8 == 0x10)
            {
                char* result_1;
                int64_t rdx_5;
                result_1 = std::__to_chars_16<uint128_t>(arg1, arg2, var_48);
                result = result_1;
            }
            else
            {
                char* result_5;
                int64_t rdx_17;
                
                if (r8 > 0x10)
                {
                    result_5 = std::__to_chars<uint128_t>(arg1, arg2, var_48, var_38);
                    result = result_5;
                }
                else if (r8 == 0xa)
                {
                    char* result_2;
                    int64_t rdx_8;
                    result_2 = std::__to_chars_10<uint128_t>(arg1, arg2, var_48);
                    result = result_2;
                }
                else if (r8 > 0xa)
                {
                    result_5 = std::__to_chars<uint128_t>(arg1, arg2, var_48, var_38);
                    result = result_5;
                }
                else if (r8 == 2)
                {
                    char* result_4;
                    int64_t rdx_14;
                    result_4 = std::__to_chars_2<uint128_t>(arg1, arg2, var_48);
                    result = result_4;
                }
                else if (r8 == 8)
                {
                    char* result_3;
                    int64_t rdx_11;
                    result_3 = std::__to_chars_8<uint128_t>(arg1, arg2, var_48);
                    result = result_3;
                }
                else
                {
                    result_5 = std::__to_chars<uint128_t>(arg1, arg2, var_48, var_38);
                    result = result_5;
                }
            }
        }
    }
    return result;
}

int32_t std::__countl_zero<uint32_t>(uint32_t arg1)
{
    int32_t var_1c = 0x20;
    
    if (!arg1)
        return 0x20;
    
    int32_t var_18_1 = 0x40;
    int32_t var_14_1 = 0x40;
    int32_t var_10_1 = 0x20;
    int32_t var_c_1 = 0;
    uint64_t rflags_1;
    int32_t temp0_1;
    temp0_1 = __bsr_gprv_memv(arg1);
    return temp0_1 ^ 0x1f;
}

char32_t* std::upper_bound<char32_t const*, char32_t>(char32_t const* arg1, char32_t const* arg2, char32_t const& arg3)
{
    return std::__upper_bound<char32_t const*, char32_t, __ops::_Val_less_iter>(arg1, arg2, arg3, 
        arg2);
}

uint32_t* std::lower_bound<uint32_t const*, uint32_t>(uint32_t const* arg1, uint32_t const* arg2, uint32_t const& arg3)
{
    return std::__lower_bound<uint32_t const*, uint32_t, __ops::_Iter_less_val>(arg1, arg2, arg3, 
        arg2);
}

char32_t* std::find<char32_t const*, char32_t>(char32_t const* arg1, char32_t const* arg2, char32_t const& arg3)
{
    return std::__find_if<char32_t const*, __ops::_Iter_equals_val<char32_t const> >(arg1, arg2, 
        __ops::__iter_equals_val<char32_t const>(arg3));
}

void* decltype(::new((void*)(0)) std::locale()) std::construct_at<std::locale>(std::locale*)(void* arg1)
{
    void* result = operator new(8, arg1);
    std::locale::locale();
    return result;
}

std::__format::_Seq_sink<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >&&& std::move<std::__format::_Seq_sink<std::string>&>(std::__format::_Seq_sink<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >&&& arg1) __pure
{
    return arg1;
}

std::string* std::__format::_Seq_sink<std::string>::get()
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    char* var_38 = std::span<char, 18446744073709551615ul>::begin();
    void* rsi;
    operator-<char*, std::span<char, 18446744073709551615ul> >(rsi + 0x18, &var_38);
    char* rax_8;
    int64_t rdx_1;
    rax_8 = std::span<char, 18446744073709551615ul>::first(rsi + 8);
    var_38 = rax_8;
    int64_t var_30 = rdx_1;
    int64_t rax_9;
    rax_9 = std::span<char, 18446744073709551615ul>::size();
    
    if (rax_9)
        std::__format::_Seq_sink<std::string>::_M_overflow();
    
    std::string* result;
    std::string::string(result);
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

auto std::ranges::__access::_Data::operator()<std::string&>(std::string&&& arg1)
{
    std::string&&& var_10 = arg1;
    return std::string::data();
}

auto std::ranges::__access::_Size::operator()<std::string&>(std::string&&& arg1)
{
    std::string&&& var_10 = arg1;
    return std::string::size();
}

char* std::__format::_Seq_sink<std::string>::view()
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    char* result_1 = std::span<char, 18446744073709551615ul>::begin();
    void* rdi;
    operator-<char*, std::span<char, 18446744073709551615ul> >(rdi + 0x18, &result_1);
    char* result_2;
    int64_t rdx_1;
    result_2 = std::span<char, 18446744073709551615ul>::first(rdi + 8);
    int64_t var_40 = rdx_1;
    int64_t rax_10;
    rax_10 = std::string::size();
    char* result;
    
    if (!rax_10)
        result = result_2;
    else
    {
        int64_t rax_11;
        rax_11 = std::span<char, 18446744073709551615ul>::size();
        
        if (rax_11)
            std::__format::_Seq_sink<std::string>::_M_overflow();
        
        std::span<char, 18446744073709551615ul>::span<std::string&>(&result_1);
        result = result_1;
    }
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

bool std::operator==<char>(std::basic_string_view<char> arg1, std::type_identity<std::basic_string_view<char> >::type arg2)
{
    int64_t var_28 = arg1;
    int64_t var_20 = arg2;
    int64_t rcx;
    int64_t var_30 = rcx;
    
    if (std::basic_string_view<char>::size() == std::basic_string_view<char>::size()
            && !std::basic_string_view<char>::compare(&var_28))
        return 1;
    
    return 0;
}

int64_t std::string::data()
{
    return std::string::_M_data();
}

std::enable_if<std::chrono::__is_duration<std::chrono::duration<int64_t, std::ratio<1l, 1000000l> > >::value, std::chrono::duration<int64_t, std::ratio<1l, 1000000l> > >::type std::chrono::duration_cast<std::chrono::duration<int64_t, std::ratio<1l, 1000000l> >, int64_t, std::ratio<1l, 1000000000l> >(std::chrono::duration<int64_t, std::ratio<1l, 1000000000l> > const& arg1)
{
    return std::chrono::__duration_cast_impl<std::chrono::duration<int64_t, std::ratio<1l, 1000000l> >, std::ratio<1l, 1000l>, int64_t, true, false>::__cast<int64_t, std::ratio<1l, 1000000000l> >(
        arg1);
}

telemetry_board::TcpConnection&&& std::move<telemetry_board::TcpConnection&>(telemetry_board::TcpConnection&&& arg1) __pure
{
    return arg1;
}

int64_t std::println<>(std::basic_format_string<char, std::type_identity<char>::type> arg1, char&& arg2)
{
    return std::println<>(*stdout, arg1, arg2);
}

uint64_t const& std::max<uint64_t>(uint64_t const& arg1, uint64_t const& arg2)
{
    if (*arg1 >= *arg2)
        return arg1;
    
    return arg2;
}

void* CyclicBuffer<canzero_frame, 32ul>::CyclicBuffer()
{
    void* s;
    __builtin_memset(s, 0, 0x200);
    *(s + 0x200) = 0;
    *(s + 0x208) = 0;
    return s;
}

void* CyclicBuffer<Timestamped<canzero_frame>, 32ul>::CyclicBuffer()
{
    int64_t i = 0x1f;
    void* result;
    void* result_1 = result;
    
    while (i >= 0)
    {
        Timestamped<canzero_frame>::Timestamped();
        i -= 1;
        result_1 += 0x14;
    }
    
    *(result + 0x280) = 0;
    *(result + 0x288) = 0;
    return result;
}

int64_t std::println<uint16_t>(std::basic_format_string<char, std::type_identity<uint16_t>::type> arg1, uint16_t&& arg2)
{
    return std::println<uint16_t>(*stdout, arg1, arg2);
}

std::pair<uint16_t, char const*> std::__format::__parse_arg_id<char>(char const* arg1, char const* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int32_t rax_5;
    int32_t var_34;
    int64_t var_28;
    
    if (*arg1 == 0x30)
    {
        var_34 = 0;
        void* var_30 = &arg1[1];
        std::pair<uint16_t, char const*>::pair<int32_t, char const*>(&var_28, &var_34);
        rax_5 = var_28;
    }
    else if (*arg1 <= 0x30 || *arg1 > 0x39)
    {
        var_34 = 0;
        int64_t var_30_2 = 0;
        std::pair<uint16_t, char const*>::pair<int32_t, std::nullptr_t>(&var_28, &var_34);
        rax_5 = var_28;
    }
    else
    {
        var_34 = *arg1 - 0x30;
        int64_t rdx_3;
        
        if (arg2 == &arg1[1] || arg1[1] <= 0x2f || arg1[1] > 0x39)
        {
            std::pair<uint16_t, char const*>::pair(&var_28, &var_34);
            rax_5 = var_28;
        }
        else
            rax_5 = std::__format::__parse_integer<char>(arg1, arg2);
    }
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return rax_5;
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::basic_format_parse_context<char>::check_arg_id(uint64_t arg1)
{
    if (*(arg1 + 0x10) == 2)
    {
        std::__format::__conflicting_indexing_in_format_string();
        /* no return */
    }
    
    *(arg1 + 0x10) = 1;
    return std::is_constant_evaluated();
}

int64_t auto std::ranges::__access::_Begin::operator()<std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1>&>(std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1>&) const(int64_t arg1, int64_t* arg2)
{
    int64_t var_10 = arg1;
    return std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1>::begin() const(
        arg2);
}

int64_t auto std::ranges::__access::_End::operator()<std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1>&>(std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1>&) const(int64_t arg1, void* arg2)
{
    int64_t var_10 = arg1;
    return 
        std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1>::end() const(
        arg2);
}

int64_t std::__unicode::_Utf_view<char32_t, std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1> >::empty() const(int64_t* arg1)
{
    return bool std::ranges::__access::_Empty::operator()<std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1> const&>(std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1> const&) const(
        &std::ranges::_Cpo::empty, arg1);
}

int64_t bool std::ranges::__access::_Empty::operator()<std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1> const&>(std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1> const&) const(int64_t arg1, int64_t* arg2)
{
    int64_t var_10 = arg1;
    return std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1>::empty() const(
        arg2);
}

int64_t std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1>::empty() const(int64_t* arg1)
{
    int64_t result;
    result = *arg1 == arg1[1];
    return result;
}

char* auto std::__unicode::_Utf_view<char32_t, std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1>>::_M_begin<char const*, char const*>(char const*, char const*)(char* arg1, int64_t arg2, char* arg3)
{
    int64_t var_18 = arg2;
    std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_Utf_iterator(
        arg1, arg3, arg3);
    return arg1;
}

char* std::__unicode::_Utf_view<char32_t, std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1> >::begin()(char* arg1, int64_t* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    auto std::ranges::__access::_End::operator()<std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1>&>(std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1>&) const(
        &std::ranges::_Cpo::end, arg2);
    auto std::__unicode::_Utf_view<char32_t, std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1>>::_M_begin<char const*, char const*>(char const*, char const*)(
        arg1, arg2, 
        auto std::ranges::__access::_Begin::operator()<std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1>&>(std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1>&) const(
            &std::ranges::_Cpo::begin, arg2));
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return arg1;
    
    __stack_chk_fail();
    /* no return */
}

void* std::__format::_Spec<char>::_M_parse_fill_and_align(char const* arg1, char const* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    void* result;
    
    if (*arg2 == 0x7b)
        result = arg2;
    else
    {
        char** var_78;
        int64_t rdx;
        std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1>::subrange<char const*>(char const*, char const*)(
            &var_78, arg2, rdx);
        void var_88;
        std::__unicode::_Utf_view<char32_t, std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1> >::_Utf_view(std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1>&&)(
            &var_88, &var_78);
        
        if (std::__unicode::_Utf_view<char32_t, std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1> >::empty() const(
            &var_88) == 1)
        {
            label_40981f:
            int32_t rax_24 = std::__format::_Spec<char>::_S_align(*arg2);
            
            if (!rax_24)
                result = arg2;
            else
            {
                *(arg1 + 8) = 0x20;
                *arg1 = (*arg1 & 0xfc) | (rax_24 & 3);
                result = &arg2[1];
            }
        }
        else
        {
            char var_68[0x30];
            std::__unicode::_Utf_view<char32_t, std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1> >::begin()(
                &var_68, &var_88);
            void var_38;
            std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::operator++(
                &var_38);
            char32_t rax_5 = std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::operator*();
            
            if (!std::__unicode::__is_scalar_value(rax_5))
                goto label_40981f;
            
            char* rax_8 = std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::base();
            
            if (rax_8 == rdx)
                goto label_40981f;
            
            int32_t rax_13 = std::__format::_Spec<char>::_S_align(*rax_8);
            
            if (!rax_13)
                goto label_40981f;
            
            *(arg1 + 8) = rax_5;
            *arg1 = (*arg1 & 0xfc) | (rax_13 & 3);
            result = &rax_8[1];
        }
    }
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

void* std::__format::_Spec<char>::_M_parse_sign(char const* arg1, char const* arg2)
{
    int64_t rdx;
    int64_t var_30 = rdx;
    int32_t rax_3 = std::__format::_Spec<char>::_S_sign(*arg2);
    
    if (!rax_3)
        return arg2;
    
    *arg1 = (*arg1 & 0xf3) | (rax_3 & 3) << 2;
    return &arg2[1];
}

char const* std::__format::_Spec<char>::_M_parse_alternate_form(char const* arg1, char const* arg2)
{
    int64_t rdx;
    int64_t var_20 = rdx;
    
    if (*arg2 != 0x23)
        return arg2;
    
    *arg1 |= 0x10;
    return &arg2[1];
}

char const* std::__format::_Spec<char>::_M_parse_zero_fill(char const* arg1, char const* arg2)
{
    int64_t rdx;
    int64_t var_20 = rdx;
    
    if (*arg2 != 0x30)
        return arg2;
    
    *arg1 |= 0x40;
    return &arg2[1];
}

char* std::__format::_Spec<char>::_M_parse_width(char const* arg1, char const* arg2, std::basic_format_parse_context<char>& arg3)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    bool var_19 = false;
    
    if (*arg2 == 0x30)
    {
        *(fsbase + 0x28);
        
        if (rax == *(fsbase + 0x28))
        {
            std::__throw_format_error("format error: width must be non-…");
            /* no return */
        }
        
        __stack_chk_fail();
        /* no return */
    }
    
    std::basic_format_parse_context<char>* rcx;
    char* result =
        std::__format::_Spec<char>::_S_parse_width_or_precision(arg2, arg3, &arg1[4], &var_19, rcx);
    
    if (result != arg2)
    {
        char rdx_1;
        
        if (!var_19)
            rdx_1 = 1;
        else
            rdx_1 = 2;
        
        *arg1 = (*arg1 & 0xfe7f) | (rdx_1 & 3) << 7;
    }
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

char const* std::__format::_Spec<char>::_M_parse_locale(char const* arg1, char const* arg2)
{
    int64_t rdx;
    int64_t var_20 = rdx;
    
    if (*arg2 != 0x4c)
        return arg2;
    
    *arg1 |= 0x20;
    return &arg2[1];
}

int64_t std::_Optional_payload_base<telemetry_board::TcpConnection>::~_Optional_payload_base()
{
    int64_t result;
    return result;
}

int64_t std::_Optional_payload<telemetry_board::TcpConnection, true, false, false>::~_Optional_payload()
{
    return std::_Optional_payload_base<telemetry_board::TcpConnection>::~_Optional_payload_base();
}

int64_t std::_Optional_payload<telemetry_board::TcpConnection, false, false, false>::~_Optional_payload()
{
    std::_Optional_payload_base<telemetry_board::TcpConnection>::_M_reset();
    return std::_Optional_payload<telemetry_board::TcpConnection, true, false, false>::~_Optional_payload();
}

uint64_t std::optional<telemetry_board::TcpConnection>::has_value()
{
    return std::_Optional_base_impl<telemetry_board::TcpConnection, std::_Optional_base<telemetry_board::TcpConnection, false, false> >::_M_is_engaged();
}

int64_t std::optional<telemetry_board::TcpConnection>::operator*()
{
    return std::_Optional_base_impl<telemetry_board::TcpConnection, std::_Optional_base<telemetry_board::TcpConnection, false, false> >::_M_get();
}

int64_t CyclicBuffer<canzero_frame, 32ul>::enqueue(canzero_frame const& arg1)
{
    int64_t rax_1 = *(arg1 + 0x208);
    int64_t var_20 = rax_1 + 1;
    
    if (var_20 == 0x20)
        var_20 = 0;
    
    if (var_20 == *(arg1 + 0x200))
        return 0;
    
    void* rcx_1 = arg1 + (rax_1 << 4);
    int64_t* rsi;
    int64_t rdx_3 = rsi[1];
    *rcx_1 = *rsi;
    *(rcx_1 + 8) = rdx_3;
    *(arg1 + 0x208) = var_20;
    return 1;
}

int32_t* Timestamped<canzero_frame>::Timestamped(Timestamped<canzero_frame> const& arg1)
{
    int64_t* rsi;
    int64_t rdx = rsi[1];
    *arg1 = *rsi;
    *(arg1 + 8) = rdx;
    return Timestamp::Timestamp(arg1 + 0x10);
}

Timestamped<canzero_frame>* CyclicBuffer<Timestamped<canzero_frame>, 32ul>::dequeue()
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    void* rsi;
    int64_t rax_2 = *(rsi + 0x280);
    Timestamped<canzero_frame>* result;
    
    if (rax_2 != *(rsi + 0x288))
    {
        void var_28;
        Timestamped<canzero_frame>::Timestamped(&var_28);
        int64_t var_38_1 = rax_2 + 1;
        
        if (var_38_1 == 0x20)
            var_38_1 = 0;
        
        *(rsi + 0x280) = var_38_1;
        std::optional<Timestamped<canzero_frame> >::optional<Timestamped<canzero_frame>, true>(
            result);
    }
    else
        std::optional<Timestamped<canzero_frame> >::optional(result);
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

std::_Optional_payload<Timestamped<canzero_frame>, true, false, false>&& std::_Optional_payload<Timestamped<canzero_frame>, true, false, false>::operator=(std::_Optional_payload<Timestamped<canzero_frame>, true, false, false>&& arg1)
{
    std::_Optional_payload_base<Timestamped<canzero_frame> >::_M_move_assign(arg1);
    return arg1;
}

uint64_t std::optional<Timestamped<canzero_frame> >::has_value()
{
    return std::_Optional_base_impl<Timestamped<canzero_frame>, std::_Optional_base<Timestamped<canzero_frame>, false, false> >::_M_is_engaged();
}

uint64_t std::_Optional_base_impl<Timestamped<canzero_frame>, std::_Optional_base<Timestamped<canzero_frame>, false, false> >::_M_is_engaged()
{
    void* rdi;
    return *(rdi + 0x14);
}

int64_t std::optional<Timestamped<canzero_frame> >::value()
{
    if (std::_Optional_base_impl<Timestamped<canzero_frame>, std::_Optional_base<Timestamped<canzero_frame>, false, false> >::_M_is_engaged())
        return std::_Optional_base_impl<Timestamped<canzero_frame>, std::_Optional_base<Timestamped<canzero_frame>, false, false> >::_M_get();
    
    std::__throw_bad_optional_access();
    /* no return */
}

int64_t Timestamped<canzero_frame>::value() __pure
{
    int64_t result;
    return result;
}

Timestamped<canzero_frame>* std::optional<Timestamped<canzero_frame> >::operator->()
{
    return std::__addressof<Timestamped<canzero_frame> >(std::_Optional_base_impl<Timestamped<canzero_frame>, std::_Optional_base<Timestamped<canzero_frame>, false, false> >::_M_get());
}

Timestamp* Timestamped<canzero_frame>::timestamp()
{
    Timestamp* result;
    Timestamp::Timestamp(result);
    return result;
}

int64_t std::optional<Timestamped<canzero_frame> >::operator*()
{
    return std::_Optional_base_impl<Timestamped<canzero_frame>, std::_Optional_base<Timestamped<canzero_frame>, false, false> >::_M_get();
}

Timestamped<canzero_frame> const& Timestamped<canzero_frame>::operator=(Timestamped<canzero_frame> const& arg1)
{
    int64_t* rsi;
    int64_t rdx = rsi[1];
    *arg1 = *rsi;
    *(arg1 + 8) = rdx;
    Timestamp::operator=(arg1 + 0x10);
    return arg1;
}

int64_t CyclicBuffer<Timestamped<canzero_frame>, 32ul>::enqueue(Timestamped<canzero_frame> const& arg1)
{
    int64_t rax_1 = *(arg1 + 0x288);
    int64_t var_20 = rax_1 + 1;
    
    if (var_20 == 0x20)
        var_20 = 0;
    
    if (var_20 == *(arg1 + 0x280))
        return 0;
    
    Timestamped<canzero_frame>::operator=(arg1 + rax_1 * 0x14);
    *(arg1 + 0x288) = var_20;
    return 1;
}

void* CyclicBuffer<Timestamped<canzero_frame>, 32ul>::clear()
{
    void* result;
    *(result + 0x280) = 0;
    *(result + 0x288) = 0;
    return result;
}

canzero_frame* CyclicBuffer<canzero_frame, 32ul>::dequeue()
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    void* rsi;
    int64_t rax_2 = *(rsi + 0x200);
    canzero_frame* result;
    
    if (rax_2 != *(rsi + 0x208))
    {
        void* rax_8 = rsi + (rax_2 << 4);
        int64_t var_28_1 = *rax_8;
        int64_t var_20_1 = *(rax_8 + 8);
        int64_t var_38_1 = rax_2 + 1;
        
        if (var_38_1 == 0x20)
            var_38_1 = 0;
        
        *(rsi + 0x200) = var_38_1;
        std::optional<canzero_frame>::optional<canzero_frame, true>(result);
    }
    else
        std::optional<canzero_frame>::optional(result);
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

uint64_t std::optional<canzero_frame>::has_value()
{
    return std::_Optional_base_impl<canzero_frame, std::_Optional_base<canzero_frame, true, true> >::_M_is_engaged();
}

int64_t std::optional<canzero_frame>::operator*()
{
    return std::_Optional_base_impl<canzero_frame, std::_Optional_base<canzero_frame, true, true> >::_M_get();
}

int64_t Timestamped<canzero_frame>::now(canzero_frame arg1)
{
    int64_t result = arg1;
    int64_t rdx;
    int64_t var_30 = rdx;
    void* fsbase;
    int64_t rax_2 = *(fsbase + 0x28);
    Timestamp::now();
    int64_t rsi;
    Timestamped<canzero_frame>::Timestamped(result, rsi);
    *(fsbase + 0x28);
    
    if (rax_2 == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

std::remove_reference<char const&>::type& std::forward<char const&>(std::remove_reference<char const&>::type& arg1) __pure
{
    return arg1;
}

void* decltype(::new((void*)(0)) char(declval<char&>())) std::construct_at<char, char&>(char*, char&)(void* arg1, std::remove_reference<char&>::type* arg2)
{
    void* result = operator new(1, arg1);
    *result = *std::forward<char&>(arg2);
    return result;
}

uint64_t std::string::_M_length(uint64_t arg1)
{
    int64_t rsi;
    *(arg1 + 8) = rsi;
    return arg1;
}

uint64_t std::string::_M_dispose()
{
    uint64_t result = std::string::_M_is_local() ^ 1;
    
    if (!result)
        return result;
    
    uint64_t rdi;
    *(rdi + 0x10);
    return std::string::_M_destroy(rdi);
}

char* std::string::_M_local_data()
{
    void* rdi;
    return std::__ptr_traits_ptr_to<char const*, char const, false>::pointer_to(rdi + 0x10);
}

char* std::string::_M_data(char* arg1)
{
    int64_t rsi;
    *arg1 = rsi;
    return arg1;
}

uint64_t std::string::_M_capacity(uint64_t arg1)
{
    int64_t rsi;
    *(arg1 + 0x10) = rsi;
    return arg1;
}

std::string&&& std::move<std::string&>(std::string&&& arg1) __pure
{
    return arg1;
}

uint32_t std::__to_chars_len_2<uint32_t>(uint32_t arg1)
{
    return std::__bit_width<uint32_t>(arg1);
}

std::to_chars_result std::__to_chars_16<uint32_t>(char* arg1, char* arg2, uint32_t arg3)
{
    uint32_t i = arg3;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    uint32_t rax_4 = (std::__to_chars_len_2<uint32_t>(i) + 3) >> 2;
    uint64_t rax_7;
    rax_7 = arg2 - arg1 < rax_4;
    char* result;
    
    if (!rax_7)
    {
        int64_t var_28;
        __builtin_strncpy(&var_28, "0123456789abcdef", 0x10);
        int32_t var_48_1 = rax_4 - 1;
        
        while (i > 0xff)
        {
            uint32_t var_5c = i >> 4;
            arg1[var_48_1] = *(&var_28 + (i & 0xf));
            i = var_5c >> 4;
            arg1[var_48_1 - 1] = *(&var_28 + (var_5c & 0xf));
            var_48_1 -= 2;
        }
        
        if (i <= 0xf)
            *arg1 = *(&var_28 + i);
        else
        {
            arg1[1] = *(&var_28 + (i & 0xf));
            *arg1 = *(&var_28 + (i >> 4));
        }
        
        int32_t var_30_1 = 0;
        result = &arg1[rax_4];
    }
    else
    {
        int32_t var_30 = 0x4b;
        result = arg2;
    }
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

std::to_chars_result std::__to_chars_10<uint32_t>(char* arg1, char* arg2, uint32_t arg3)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    uint32_t rax_2 = std::__to_chars_len<uint32_t>(arg3, 0xa);
    uint64_t rax_5;
    rax_5 = arg2 - arg1 < rax_2;
    char* result;
    
    if (!rax_5)
    {
        std::__to_chars_10_impl<uint32_t>(arg1, rax_2, arg3);
        int32_t var_20_1 = 0;
        result = &arg1[rax_2];
    }
    else
    {
        int32_t var_20 = 0x4b;
        result = arg2;
    }
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

std::to_chars_result std::__to_chars_8<uint32_t>(char* arg1, char* arg2, uint32_t arg3)
{
    uint32_t i = arg3;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int32_t var_34 = 0;
    uint32_t rax_6 = (std::__to_chars_len_2<uint32_t>(i) + 2) / 3;
    uint64_t rax_9;
    rax_9 = arg2 - arg1 < rax_6;
    char* result;
    
    if (!rax_9)
    {
        int32_t var_38_1 = rax_6 - 1;
        
        while (i > 0x3f)
        {
            uint32_t var_4c = i >> 3;
            arg1[var_38_1] = (i & 7) + 0x30;
            i = var_4c >> 3;
            arg1[var_38_1 - 1] = (var_4c & 7) + 0x30;
            var_38_1 -= 2;
        }
        
        if (i <= 7)
            *arg1 = i + 0x30;
        else
        {
            arg1[1] = (i & 7) + 0x30;
            *arg1 = (i >> 3) + 0x30;
        }
        
        int32_t var_20_1 = 0;
        result = &arg1[rax_6];
    }
    else
    {
        int32_t var_20 = 0x4b;
        result = arg2;
    }
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

std::to_chars_result std::__to_chars_2<uint32_t>(char* arg1, char* arg2, uint32_t arg3)
{
    uint32_t var_4c = arg3;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    uint32_t rax_2 = std::__to_chars_len_2<uint32_t>(var_4c);
    uint64_t rax_5;
    rax_5 = arg2 - arg1 < rax_2;
    char* result;
    
    if (!rax_5)
    {
        int32_t i = rax_2 - 1;
        
        while (i)
        {
            int32_t i_1 = i;
            i = i_1 - 1;
            arg1[i_1] = (var_4c & 1) + 0x30;
            var_4c u>>= 1;
        }
        
        *arg1 = 0x31;
        int32_t var_20_1 = 0;
        result = &arg1[rax_2];
    }
    else
    {
        int32_t var_20 = 0x4b;
        result = arg2;
    }
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

std::to_chars_result std::__to_chars<uint32_t>(char* arg1, char* arg2, uint32_t arg3, int32_t arg4)
{
    uint32_t i = arg3;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    uint32_t rax_2 = std::__to_chars_len<uint32_t>(i, arg4);
    uint64_t rax_5;
    rax_5 = arg2 - arg1 < rax_2;
    char* result;
    
    if (!rax_5)
    {
        int32_t var_58_1 = rax_2 - 1;
        int64_t var_38;
        __builtin_strncpy(&var_38, "0123456789abcdefghijklmnopqrstuvwxyz", 0x24);
        
        for (; i >= arg4; i = COMBINE(0, i) / arg4)
        {
            int32_t rax_14 = var_58_1;
            var_58_1 = rax_14 - 1;
            arg1[rax_14] = *(&var_38 + COMBINE(0, i) % arg4);
        }
        
        *arg1 = *(&var_38 + i);
        int32_t var_40_1 = 0;
        result = &arg1[rax_2];
    }
    else
    {
        int32_t var_40 = 0x4b;
        result = arg2;
    }
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

uint32_t std::__to_chars_len_2<uint64_t>(uint64_t arg1)
{
    return std::__bit_width<uint64_t>(arg1);
}

std::to_chars_result std::__to_chars_16<uint64_t>(char* arg1, char* arg2, uint64_t arg3)
{
    uint64_t i = arg3;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    uint32_t rax_4 = (std::__to_chars_len_2<uint64_t>(i) + 3) >> 2;
    uint64_t rax_7;
    rax_7 = arg2 - arg1 < rax_4;
    char* result;
    
    if (!rax_7)
    {
        int64_t var_28;
        __builtin_strncpy(&var_28, "0123456789abcdef", 0x10);
        int32_t var_50_1 = rax_4 - 1;
        
        while (i > 0xff)
        {
            uint64_t var_70 = i >> 4;
            arg1[var_50_1] = *((i & 0xf) + &var_28);
            i = var_70 >> 4;
            arg1[var_50_1 - 1] = *((var_70 & 0xf) + &var_28);
            var_50_1 -= 2;
        }
        
        if (i <= 0xf)
            *arg1 = *(i + &var_28);
        else
        {
            arg1[1] = *((i & 0xf) + &var_28);
            *arg1 = *((i >> 4) + &var_28);
        }
        
        int32_t var_30_1 = 0;
        result = &arg1[rax_4];
    }
    else
    {
        int32_t var_30 = 0x4b;
        result = arg2;
    }
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

std::to_chars_result std::__to_chars_10<uint64_t>(char* arg1, char* arg2, uint64_t arg3)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    uint32_t rax_2 = std::__to_chars_len<uint64_t>(arg3, 0xa);
    uint64_t rax_5;
    rax_5 = arg2 - arg1 < rax_2;
    char* result;
    
    if (!rax_5)
    {
        std::__to_chars_10_impl<uint64_t>(arg1, rax_2, arg3);
        int32_t var_20_1 = 0;
        result = &arg1[rax_2];
    }
    else
    {
        int32_t var_20 = 0x4b;
        result = arg2;
    }
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

std::to_chars_result std::__to_chars_8<uint64_t>(char* arg1, char* arg2, uint64_t arg3)
{
    uint64_t i = arg3;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int32_t var_3c = 0;
    uint32_t rax_6 = (std::__to_chars_len_2<uint64_t>(i) + 2) / 3;
    uint64_t rax_9;
    rax_9 = arg2 - arg1 < rax_6;
    char* result;
    
    if (!rax_9)
    {
        int32_t var_40_1 = rax_6 - 1;
        
        while (i > 0x3f)
        {
            uint64_t var_60 = i >> 3;
            arg1[var_40_1] = (i & 7) + 0x30;
            i = var_60 >> 3;
            arg1[var_40_1 - 1] = (var_60 & 7) + 0x30;
            var_40_1 -= 2;
        }
        
        if (i <= 7)
            *arg1 = i + 0x30;
        else
        {
            arg1[1] = (i & 7) + 0x30;
            *arg1 = (i >> 3) + 0x30;
        }
        
        int32_t var_20_1 = 0;
        result = &arg1[rax_6];
    }
    else
    {
        int32_t var_20 = 0x4b;
        result = arg2;
    }
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

std::to_chars_result std::__to_chars_2<uint64_t>(char* arg1, char* arg2, uint64_t arg3)
{
    uint64_t var_50 = arg3;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    uint32_t rax_2 = std::__to_chars_len_2<uint64_t>(var_50);
    uint64_t rax_5;
    rax_5 = arg2 - arg1 < rax_2;
    char* result;
    
    if (!rax_5)
    {
        int32_t i = rax_2 - 1;
        
        while (i)
        {
            int32_t i_1 = i;
            i = i_1 - 1;
            arg1[i_1] = (var_50 & 1) + 0x30;
            var_50 u>>= 1;
        }
        
        *arg1 = 0x31;
        int32_t var_20_1 = 0;
        result = &arg1[rax_2];
    }
    else
    {
        int32_t var_20 = 0x4b;
        result = arg2;
    }
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

std::to_chars_result std::__to_chars<uint64_t>(char* arg1, char* arg2, uint64_t arg3, int32_t arg4)
{
    uint64_t i = arg3;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    uint32_t rax_2 = std::__to_chars_len<uint64_t>(i, arg4);
    uint64_t rax_5;
    rax_5 = arg2 - arg1 < rax_2;
    char* result;
    
    if (!rax_5)
    {
        int32_t var_60_1 = rax_2 - 1;
        int64_t var_38;
        __builtin_strncpy(&var_38, "0123456789abcdefghijklmnopqrstuvwxyz", 0x24);
        
        for (; i >= arg4; i = COMBINE(0, i) / arg4)
        {
            int32_t rax_17 = var_60_1;
            var_60_1 = rax_17 - 1;
            arg1[rax_17] = *(COMBINE(0, i) % arg4 + &var_38);
        }
        
        *arg1 = *(i + &var_38);
        int32_t var_40_1 = 0;
        result = &arg1[rax_2];
    }
    else
    {
        int32_t var_40 = 0x4b;
        result = arg2;
    }
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

uint32_t std::__to_chars_len_2<uint128_t>(uint128_t arg1)
{
    int64_t rdi;
    return std::__bit_width<uint128_t>(rdi);
}

std::to_chars_result std::__to_chars_16<uint128_t>(char* arg1, char* arg2, uint128_t arg3)
{
    uint64_t rdx;
    uint64_t var_88 = rdx;
    uint64_t rcx;
    uint64_t var_80 = rcx;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    uint32_t rax_4 = (std::__to_chars_len_2<uint128_t>(var_88) + 3) >> 2;
    uint64_t rax_7;
    rax_7 = arg2 - arg1 < rax_4;
    char* result;
    
    if (!rax_7)
    {
        int64_t var_28;
        __builtin_strncpy(&var_28, "0123456789abcdef", 0x10);
        int32_t var_60_1 = rax_4 - 1;
        
        while (true)
        {
            bool c_2 = /* bool c_2 = unimplemented  {sbb rax, qword [rbp-0x78]} */;
            
            if (!c_2)
                break;
            
            int64_t var_30_1 = 0;
            int64_t rax_15 = var_88 >> 4 | var_80 << 0x3c;
            uint64_t rdx_5 = var_80 >> 4;
            arg1[var_60_1] = *((var_88 & 0xf) + &var_28);
            int64_t var_30_2 = 0;
            var_88 = rax_15 >> 4 | rdx_5 << 0x3c;
            var_80 = rdx_5 >> 4;
            arg1[var_60_1 - 1] = *((rax_15 & 0xf) + &var_28);
            var_60_1 -= 2;
        }
        
        bool c_4 = /* bool c_4 = unimplemented  {sbb rax, qword [rbp-0x78]} */;
        
        if (!c_4)
            *arg1 = *(var_88 + &var_28);
        else
        {
            int64_t var_40_1 = 0;
            uint64_t var_80_2 = var_80 >> 4;
            arg1[1] = *((var_88 & 0xf) + &var_28);
            *arg1 = *((var_88 >> 4 | var_80 << 0x3c) + &var_28);
        }
        
        int32_t var_50_1 = 0;
        result = &arg1[rax_4];
    }
    else
    {
        int32_t var_50 = 0x4b;
        result = arg2;
    }
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

uint32_t std::__to_chars_len<uint128_t>(uint128_t arg1, int32_t arg2)
{
    int64_t var_48 = arg2;
    int64_t rsi;
    int64_t var_40 = rsi;
    int32_t result = 1;
    
    while (true)
    {
        bool c_2 = /* bool c_2 = unimplemented  {sbb rcx, rdx} */;
        
        if (c_2)
            return result;
        
        bool c_4 = /* bool c_4 = unimplemented  {sbb rcx, rdx} */;
        
        if (c_4)
            return result + 1;
        
        bool c_6 = /* bool c_6 = unimplemented  {sbb rcx, rdx} */;
        
        if (c_6)
            return result + 2;
        
        bool c_8 = /* bool c_8 = unimplemented  {sbb rcx, rdx} */;
        
        if (c_8)
            break;
        
        int64_t rax_20;
        int32_t rdx;
        int64_t rdx_3;
        rax_20 = __udivti3(var_48, var_40, rdx * rdx * rdx * rdx, 0);
        var_48 = rax_20;
        var_40 = rdx_3;
        result += 4;
    }
    
    return result + 3;
}

std::to_chars_result std::__to_chars_10<uint128_t>(char* arg1, char* arg2, uint128_t arg3)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t rdx;
    uint32_t rax_2 = std::__to_chars_len<uint128_t>(rdx, rdx);
    uint64_t rax_5;
    rax_5 = arg2 - arg1 < rax_2;
    char* result;
    
    if (!rax_5)
    {
        std::__to_chars_10_impl<uint128_t>(arg1, rax_2);
        int32_t var_20_1 = 0;
        result = &arg1[rax_2];
    }
    else
    {
        int32_t var_20 = 0x4b;
        result = arg2;
    }
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

std::to_chars_result std::__to_chars_8<uint128_t>(char* arg1, char* arg2, uint128_t arg3)
{
    uint64_t rdx;
    uint64_t var_78 = rdx;
    uint64_t rcx;
    uint64_t var_70 = rcx;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int32_t var_4c = 0;
    uint32_t rax_6 = (std::__to_chars_len_2<uint128_t>(var_78) + 2) / 3;
    uint64_t rax_9;
    rax_9 = arg2 - arg1 < rax_6;
    char* result;
    
    if (!rax_9)
    {
        int32_t var_50_1 = rax_6 - 1;
        
        while (true)
        {
            bool c_2 = /* bool c_2 = unimplemented  {sbb rax, qword [rbp-0x68]} */;
            
            if (!c_2)
                break;
            
            int64_t var_20_1 = 0;
            int64_t rax_17 = var_78 >> 3 | var_70 << 0x3d;
            uint64_t rdx_6 = var_70 >> 3;
            arg1[var_50_1] = (var_78 & 7) + 0x30;
            int64_t var_20_2 = 0;
            var_78 = rax_17 >> 3 | rdx_6 << 0x3d;
            var_70 = rdx_6 >> 3;
            arg1[var_50_1 - 1] = (rax_17 & 7) + 0x30;
            var_50_1 -= 2;
        }
        
        bool c_4 = /* bool c_4 = unimplemented  {sbb rax, qword [rbp-0x68]} */;
        
        if (!c_4)
            *arg1 = var_78 + 0x30;
        else
        {
            int64_t var_30_1 = 0;
            uint64_t var_70_2 = var_70 >> 3;
            arg1[1] = (var_78 & 7) + 0x30;
            *arg1 = (var_78 >> 3 | var_70 << 0x3d) + 0x30;
        }
        
        int32_t var_40_1 = 0;
        result = &arg1[rax_6];
    }
    else
    {
        int32_t var_40 = 0x4b;
        result = arg2;
    }
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

std::to_chars_result std::__to_chars_2<uint128_t>(char* arg1, char* arg2, uint128_t arg3)
{
    int64_t rdx;
    int64_t var_58 = rdx;
    uint64_t rcx;
    uint64_t var_50 = rcx;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    uint32_t rax_2 = std::__to_chars_len_2<uint128_t>(var_58);
    uint64_t rax_5;
    rax_5 = arg2 - arg1 < rax_2;
    char* result;
    
    if (!rax_5)
    {
        int32_t i = rax_2 - 1;
        
        while (i)
        {
            int32_t i_1 = i;
            i = i_1 - 1;
            arg1[i_1] = (var_58 & 1) + 0x30;
            var_58 = var_58 >> 1 | var_50 << 0x3f;
            var_50 u>>= 1;
        }
        
        *arg1 = 0x31;
        int32_t var_20_1 = 0;
        result = &arg1[rax_2];
    }
    else
    {
        int32_t var_20 = 0x4b;
        result = arg2;
    }
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

std::to_chars_result std::__to_chars<uint128_t>(char* arg1, char* arg2, uint128_t arg3, int32_t arg4)
{
    int64_t var_a8 = arg4;
    int64_t rcx;
    int64_t var_a0 = rcx;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    void var_b8;
    uint32_t rax_2 = std::__to_chars_len<uint128_t>(var_b8, var_a8);
    uint64_t rax_5;
    rax_5 = arg2 - arg1 < rax_2;
    char* result;
    
    if (!rax_5)
    {
        int32_t var_80_1 = rax_2 - 1;
        int64_t var_48;
        __builtin_strncpy(&var_48, "0123456789abcdefghijklmnopqrstuvwxyz", 0x24);
        
        while (true)
        {
            bool c_2 = /* bool c_2 = unimplemented  {sbb rcx, rdx} */;
            
            if (c_2)
                break;
            
            int64_t rax_12;
            int64_t rdx_3;
            int32_t r8;
            rdx_3 = HIGHQ(r8);
            rax_12 = LOWQ(r8);
            int64_t rax_13;
            int64_t rdx_5;
            rax_13 = __udivti3(var_a8, var_a0, rax_12, rdx_3);
            int64_t rax_16;
            int64_t rdx_6;
            rdx_6 = HIGHQ(r8);
            rax_16 = LOWQ(r8);
            int64_t rax_17;
            int64_t rdx_8;
            rax_17 = __umodti3(var_a8, var_a0, rax_16, rdx_6);
            int64_t var_50_1 = rdx_8;
            int32_t rax_20 = var_80_1;
            var_80_1 = rax_20 - 1;
            arg1[rax_20] = *(rax_17 + &var_48);
            var_a8 = rax_13;
            var_a0 = rdx_5;
        }
        
        *arg1 = *(var_a8 + &var_48);
        int32_t var_70_1 = 0;
        result = &arg1[rax_2];
    }
    else
    {
        int32_t var_70 = 0x4b;
        result = arg2;
    }
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

char32_t const* std::__upper_bound<char32_t const*, char32_t, __ops::_Val_less_iter>(char32_t const* arg1, char32_t const* arg2, char32_t const& arg3, __ops::_Val_less_iter arg4)
{
    char32_t const* result = arg1;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    char32_t const* result_1 = result;
    int64_t i = (arg2 - result_1) >> 2;
    
    while (i > 0)
    {
        int64_t i_1 = i >> 1;
        result_1 = result;
        char32_t const** var_18_1 = &result_1;
        std::__advance<char32_t const*, int64_t>(&result_1, i_1, i_1);
        void var_71;
        
        if (!__ops::_Val_less_iter::operator()<char32_t const, char32_t const*>(&var_71, arg3))
        {
            result = &result_1[1];
            i = i - i_1 - 1;
        }
        else
            i = i_1;
    }
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

uint32_t const* std::__lower_bound<uint32_t const*, uint32_t, __ops::_Iter_less_val>(uint32_t const* arg1, uint32_t const* arg2, uint32_t const& arg3, __ops::_Iter_less_val arg4)
{
    uint32_t const* result = arg1;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    uint32_t const* result_1 = result;
    int64_t i = (arg2 - result_1) >> 2;
    
    while (i > 0)
    {
        int64_t i_1 = i >> 1;
        result_1 = result;
        uint32_t const** var_18_1 = &result_1;
        std::__advance<uint32_t const*, int64_t>(&result_1, i_1, i_1);
        void var_71;
        
        if (!__ops::_Iter_less_val::operator()<uint32_t const*, uint32_t const>(&var_71, result_1))
            i = i_1;
        else
        {
            result = &result_1[1];
            i = i - i_1 - 1;
        }
    }
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

__gnu_cxx::__ops::_Iter_equals_val<char32_t const> __ops::__iter_equals_val<char32_t const>(char32_t const& arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t result;
    __ops::_Iter_equals_val<char32_t const>::_Iter_equals_val(&result);
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

char32_t* std::__find_if<char32_t const*, __ops::_Iter_equals_val<char32_t const> >(char32_t const* arg1, char32_t const* arg2, __gnu_cxx::__ops::_Iter_equals_val<char32_t const> arg3)
{
    return std::__find_if<char32_t const*, __ops::_Iter_equals_val<char32_t const> >(arg1, arg2, 
        arg3, arg2);
}

std::__format::_Sink_iter<char>&&& std::move<std::__format::_Sink_iter<char>&>(std::__format::_Sink_iter<char>&&& arg1) __pure
{
    return arg1;
}

int64_t auto std::__format::_Sink_iter<char> std::__format::__do_vformat_to<std::__format::_Sink_iter<char>, char, std::basic_format_context<std::__format::_Sink_iter<char>, char>>(std::__format::_Sink_iter<char>, std::basic_string_view<char, std::char_traits<char>>, std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char>> const&, std::locale const*)::'lambda'(std::__format::_Sink_iter<char>&)::operator()<std::monostate>(std::__format::_Sink_iter<char>&) const() __pure
{
    int64_t rdi;
    int64_t var_10 = rdi;
    int64_t rsi;
    int64_t var_18 = rsi;
}

int64_t auto std::__format::_Sink_iter<char> std::__format::__do_vformat_to<std::__format::_Sink_iter<char>, char, std::basic_format_context<std::__format::_Sink_iter<char>, char>>(std::__format::_Sink_iter<char>, std::basic_string_view<char, std::char_traits<char>>, std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char>> const&, std::locale const*)::'lambda'(std::__format::_Sink_iter<char>&)::operator()<bool>(std::__format::_Sink_iter<char>&) const(int64_t* arg1, char* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t rax_6 = (*arg2 ^ 1) + 4;
    char const* const var_38 = "false";
    char const* const var_30 = "true";
    int64_t var_48 = std::__format::_Sink_iter<char>::_M_reserve(*arg1);
    
    if (std::__format::_Sink<char>::_Reservation::operator bool())
    {
        int64_t rbx_1 = (&var_38)[*arg2];
        int64_t rax_15 = std::__format::_Sink<char>::_Reservation::get();
        memcpy(rax_15, rbx_1, rax_6, rax_15);
        std::__format::_Sink<char>::_Reservation::_M_bump(&var_48);
        *arg1[1] = 1;
    }
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::__format::_Sink_iter<char>::_M_reserve(uint64_t arg1)
{
    int64_t rsi;
    return (*(**arg1 + 8))(*arg1, rsi);
}

int64_t auto std::__format::_Sink_iter<char> std::__format::__do_vformat_to<std::__format::_Sink_iter<char>, char, std::basic_format_context<std::__format::_Sink_iter<char>, char>>(std::__format::_Sink_iter<char>, std::basic_string_view<char, std::char_traits<char>>, std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char>> const&, std::locale const*)::'lambda'(std::__format::_Sink_iter<char>&)::operator()<char>(std::__format::_Sink_iter<char>&) const(int64_t* arg1, char* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_28 = std::__format::_Sink_iter<char>::_M_reserve(*arg1);
    
    if (std::__format::_Sink<char>::_Reservation::operator bool())
    {
        *std::__format::_Sink<char>::_Reservation::get() = *arg2;
        std::__format::_Sink<char>::_Reservation::_M_bump(&var_28);
        *arg1[1] = 1;
    }
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t auto std::__format::_Sink_iter<char> std::__format::__do_vformat_to<std::__format::_Sink_iter<char>, char, std::basic_format_context<std::__format::_Sink_iter<char>, char>>(std::__format::_Sink_iter<char>, std::basic_string_view<char, std::char_traits<char>>, std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char>> const&, std::locale const*)::'lambda'(std::__format::_Sink_iter<char>&)::operator()<int>(std::__format::_Sink_iter<char>&) const(int64_t* arg1, int32_t* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    uint8_t rax_3 = *arg2 >> 0x1f;
    int32_t var_28;
    
    if (!rax_3)
        var_28 = *arg2;
    else
        var_28 = -(*arg2);
    
    uint32_t rax_10 = std::__to_chars_len<uint32_t>(var_28, 0xa);
    int64_t var_20 = std::__format::_Sink_iter<char>::_M_reserve(*arg1);
    
    if (std::__format::_Sink<char>::_Reservation::operator bool())
    {
        char* rax_15 = std::__format::_Sink<char>::_Reservation::get();
        *rax_15 = 0x2d;
        std::__to_chars_10_impl<uint32_t>(&rax_15[rax_3], rax_10, var_28);
        std::__format::_Sink<char>::_Reservation::_M_bump(&var_20);
        *arg1[1] = 1;
    }
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t auto std::__format::_Sink_iter<char> std::__format::__do_vformat_to<std::__format::_Sink_iter<char>, char, std::basic_format_context<std::__format::_Sink_iter<char>, char>>(std::__format::_Sink_iter<char>, std::basic_string_view<char, std::char_traits<char>>, std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char>> const&, std::locale const*)::'lambda'(std::__format::_Sink_iter<char>&)::operator()<unsigned int>(std::__format::_Sink_iter<char>&) const(int64_t* arg1, int32_t* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    char var_29 = 0;
    uint32_t rax_2 = *arg2;
    uint32_t rax_4 = std::__to_chars_len<uint32_t>(rax_2, 0xa);
    int64_t var_20 = std::__format::_Sink_iter<char>::_M_reserve(*arg1);
    
    if (std::__format::_Sink<char>::_Reservation::operator bool())
    {
        char* rax_9 = std::__format::_Sink<char>::_Reservation::get();
        *rax_9 = 0x2d;
        std::__to_chars_10_impl<uint32_t>(rax_9, rax_4, rax_2);
        std::__format::_Sink<char>::_Reservation::_M_bump(&var_20);
        *arg1[1] = 1;
    }
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t auto std::__format::_Sink_iter<char> std::__format::__do_vformat_to<std::__format::_Sink_iter<char>, char, std::basic_format_context<std::__format::_Sink_iter<char>, char>>(std::__format::_Sink_iter<char>, std::basic_string_view<char, std::char_traits<char>>, std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char>> const&, std::locale const*)::'lambda'(std::__format::_Sink_iter<char>&)::operator()<long long>(std::__format::_Sink_iter<char>&) const(int64_t* arg1, int64_t* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    uint8_t rax_3 = *arg2 >> 0x3f;
    int64_t var_20;
    
    if (!rax_3)
        var_20 = *arg2;
    else
        var_20 = -(*arg2);
    
    uint32_t rax_10 = std::__to_chars_len<uint64_t>(var_20, 0xa);
    int64_t var_28 = std::__format::_Sink_iter<char>::_M_reserve(*arg1);
    
    if (std::__format::_Sink<char>::_Reservation::operator bool())
    {
        char* rax_15 = std::__format::_Sink<char>::_Reservation::get();
        *rax_15 = 0x2d;
        std::__to_chars_10_impl<uint64_t>(&rax_15[rax_3], rax_10, var_20);
        std::__format::_Sink<char>::_Reservation::_M_bump(&var_28);
        *arg1[1] = 1;
    }
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t auto std::__format::_Sink_iter<char> std::__format::__do_vformat_to<std::__format::_Sink_iter<char>, char, std::basic_format_context<std::__format::_Sink_iter<char>, char>>(std::__format::_Sink_iter<char>, std::basic_string_view<char, std::char_traits<char>>, std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char>> const&, std::locale const*)::'lambda'(std::__format::_Sink_iter<char>&)::operator()<unsigned long long>(std::__format::_Sink_iter<char>&) const(int64_t* arg1, int64_t* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    char var_2d = 0;
    uint64_t rax_2 = *arg2;
    uint32_t rax_4 = std::__to_chars_len<uint64_t>(rax_2, 0xa);
    int64_t var_28 = std::__format::_Sink_iter<char>::_M_reserve(*arg1);
    
    if (std::__format::_Sink<char>::_Reservation::operator bool())
    {
        char* rax_9 = std::__format::_Sink<char>::_Reservation::get();
        *rax_9 = 0x2d;
        std::__to_chars_10_impl<uint64_t>(rax_9, rax_4, rax_2);
        std::__format::_Sink<char>::_Reservation::_M_bump(&var_28);
        *arg1[1] = 1;
    }
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t auto std::__format::_Sink_iter<char> std::__format::__do_vformat_to<std::__format::_Sink_iter<char>, char, std::basic_format_context<std::__format::_Sink_iter<char>, char>>(std::__format::_Sink_iter<char>, std::basic_string_view<char, std::char_traits<char>>, std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char>> const&, std::locale const*)::'lambda'(std::__format::_Sink_iter<char>&)::operator()<float>(std::__format::_Sink_iter<char>&) const() __pure
{
    int64_t rdi;
    int64_t var_10 = rdi;
    int64_t rsi;
    int64_t var_18 = rsi;
}

int64_t auto std::__format::_Sink_iter<char> std::__format::__do_vformat_to<std::__format::_Sink_iter<char>, char, std::basic_format_context<std::__format::_Sink_iter<char>, char>>(std::__format::_Sink_iter<char>, std::basic_string_view<char, std::char_traits<char>>, std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char>> const&, std::locale const*)::'lambda'(std::__format::_Sink_iter<char>&)::operator()<double>(std::__format::_Sink_iter<char>&) const() __pure
{
    int64_t rdi;
    int64_t var_10 = rdi;
    int64_t rsi;
    int64_t var_18 = rsi;
}

int64_t auto std::__format::_Sink_iter<char> std::__format::__do_vformat_to<std::__format::_Sink_iter<char>, char, std::basic_format_context<std::__format::_Sink_iter<char>, char>>(std::__format::_Sink_iter<char>, std::basic_string_view<char, std::char_traits<char>>, std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char>> const&, std::locale const*)::'lambda'(std::__format::_Sink_iter<char>&)::operator()<long double>(std::__format::_Sink_iter<char>&) const() __pure
{
    int64_t rdi;
    int64_t var_10 = rdi;
    int64_t rsi;
    int64_t var_18 = rsi;
}

int64_t auto std::__format::_Sink_iter<char> std::__format::__do_vformat_to<std::__format::_Sink_iter<char>, char, std::basic_format_context<std::__format::_Sink_iter<char>, char>>(std::__format::_Sink_iter<char>, std::basic_string_view<char, std::char_traits<char>>, std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char>> const&, std::locale const*)::'lambda'(std::__format::_Sink_iter<char>&)::operator()<char const*>(std::__format::_Sink_iter<char>&) const(int64_t* arg1, int64_t* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    *arg2;
    char var_38[0x18];
    std::basic_string_view<char>::basic_string_view(&var_38);
    uint64_t rbx = *arg1;
    std::basic_string_view<char>::size();
    int64_t var_40 = std::__format::_Sink_iter<char>::_M_reserve(rbx);
    
    if (std::__format::_Sink<char>::_Reservation::operator bool())
    {
        int64_t rax_6 = std::basic_string_view<char>::size();
        int64_t rax_7 = std::basic_string_view<char>::data();
        memcpy(std::__format::_Sink<char>::_Reservation::get(), rax_7, rax_6);
        std::basic_string_view<char>::size();
        std::__format::_Sink<char>::_Reservation::_M_bump(&var_40);
        *arg1[1] = 1;
    }
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t auto std::__format::_Sink_iter<char> std::__format::__do_vformat_to<std::__format::_Sink_iter<char>, char, std::basic_format_context<std::__format::_Sink_iter<char>, char>>(std::__format::_Sink_iter<char>, std::basic_string_view<char, std::char_traits<char>>, std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char>> const&, std::locale const*)::'lambda'(std::__format::_Sink_iter<char>&)::operator()<std::basic_string_view<char, std::char_traits<char>>>(std::__format::_Sink_iter<char>&) const(int64_t* arg1, int64_t* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_38 = *arg2;
    int64_t var_30 = arg2[1];
    uint64_t rbx = *arg1;
    std::basic_string_view<char>::size();
    int64_t var_40 = std::__format::_Sink_iter<char>::_M_reserve(rbx);
    
    if (std::__format::_Sink<char>::_Reservation::operator bool())
    {
        int64_t rax_7 = std::basic_string_view<char>::size();
        int64_t rax_8 = std::basic_string_view<char>::data();
        memcpy(std::__format::_Sink<char>::_Reservation::get(), rax_8, rax_7);
        std::basic_string_view<char>::size();
        std::__format::_Sink<char>::_Reservation::_M_bump(&var_40);
        *arg1[1] = 1;
    }
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t auto std::__format::_Sink_iter<char> std::__format::__do_vformat_to<std::__format::_Sink_iter<char>, char, std::basic_format_context<std::__format::_Sink_iter<char>, char>>(std::__format::_Sink_iter<char>, std::basic_string_view<char, std::char_traits<char>>, std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char>> const&, std::locale const*)::'lambda'(std::__format::_Sink_iter<char>&)::operator()<void const*>(std::__format::_Sink_iter<char>&) const() __pure
{
    int64_t rdi;
    int64_t var_10 = rdi;
    int64_t rsi;
    int64_t var_18 = rsi;
}

int64_t auto std::__format::_Sink_iter<char> std::__format::__do_vformat_to<std::__format::_Sink_iter<char>, char, std::basic_format_context<std::__format::_Sink_iter<char>, char>>(std::__format::_Sink_iter<char>, std::basic_string_view<char, std::char_traits<char>>, std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char>> const&, std::locale const*)::'lambda'(std::__format::_Sink_iter<char>&)::operator()<std::basic_format_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char>>::handle>(std::__format::_Sink_iter<char>&) const() __pure
{
    int64_t rdi;
    int64_t var_10 = rdi;
    int64_t rsi;
    int64_t var_18 = rsi;
}

int64_t auto std::__format::_Sink_iter<char> std::__format::__do_vformat_to<std::__format::_Sink_iter<char>, char, std::basic_format_context<std::__format::_Sink_iter<char>, char>>(std::__format::_Sink_iter<char>, std::basic_string_view<char, std::char_traits<char>>, std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char>> const&, std::locale const*)::'lambda'(std::__format::_Sink_iter<char>&)::operator()<__int128>(std::__format::_Sink_iter<char>&) const(int64_t* arg1, int64_t* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    uint8_t rax_3 = arg2[1] >> 0x3f;
    int64_t var_28;
    int64_t var_20;
    
    if (!rax_3)
    {
        var_28 = *arg2;
        var_20 = arg2[1];
    }
    else
    {
        int64_t rax_5 = *arg2;
        var_28 = -(rax_5);
        var_20 = -((arg2[1] + 0));
    }
    uint32_t rax_10 = std::__to_chars_len<uint128_t>(arg2, var_28);
    int64_t var_38 = std::__format::_Sink_iter<char>::_M_reserve(*arg1);
    
    if (std::__format::_Sink<char>::_Reservation::operator bool())
    {
        char* rax_15 = std::__format::_Sink<char>::_Reservation::get();
        *rax_15 = 0x2d;
        std::__to_chars_10_impl<uint128_t>(&rax_15[rax_3], rax_10);
        std::__format::_Sink<char>::_Reservation::_M_bump(&var_38);
        *arg1[1] = 1;
    }
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t auto std::__format::_Sink_iter<char> std::__format::__do_vformat_to<std::__format::_Sink_iter<char>, char, std::basic_format_context<std::__format::_Sink_iter<char>, char>>(std::__format::_Sink_iter<char>, std::basic_string_view<char, std::char_traits<char>>, std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char>> const&, std::locale const*)::'lambda'(std::__format::_Sink_iter<char>&)::operator()<unsigned __int128>(std::__format::_Sink_iter<char>&) const(int64_t* arg1, int64_t* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    char var_3d = 0;
    arg2[1];
    uint32_t rax_4 = std::__to_chars_len<uint128_t>(arg2, (*arg2));
    int64_t var_38 = std::__format::_Sink_iter<char>::_M_reserve(*arg1);
    
    if (std::__format::_Sink<char>::_Reservation::operator bool())
    {
        char* rax_9 = std::__format::_Sink<char>::_Reservation::get();
        *rax_9 = 0x2d;
        std::__to_chars_10_impl<uint128_t>(rax_9, rax_4);
        std::__format::_Sink<char>::_Reservation::_M_bump(&var_38);
        *arg1[1] = 1;
    }
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t auto std::__format::_Sink_iter<char> std::__format::__do_vformat_to<std::__format::_Sink_iter<char>, char, std::basic_format_context<std::__format::_Sink_iter<char>, char>>(std::__format::_Sink_iter<char>, std::basic_string_view<char, std::char_traits<char>>, std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char>> const&, std::locale const*)::'lambda'(std::__format::_Sink_iter<char>&)::operator()<_Float128>(std::__format::_Sink_iter<char>&) const() __pure
{
    int64_t rdi;
    int64_t var_10 = rdi;
    int64_t rsi;
    int64_t var_18 = rsi;
}

int64_t decltype(auto) std::basic_format_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char>>::_M_visit<std::__format::_Sink_iter<char> std::__format::__do_vformat_to<std::__format::_Sink_iter<char>, char, std::basic_format_context<std::__format::_Sink_iter<char>, char>>(std::__format::_Sink_iter<char>, std::basic_string_view<char, std::char_traits<char>>, std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char>> const&, std::locale const*)::'lambda'(std::__format::_Sink_iter<char>&)>(std::__format::_Sink_iter<char>&&, std::__format::_Arg_t)(int64_t arg1, int64_t arg2, char arg3)
{
    int64_t var_20 = arg1;
    int64_t var_28 = arg2;
    /* jump -> *((arg3 << 2) + &data_40c640) + &data_40c640 */
}

int64_t decltype(auto) std::visit_format_arg<std::__format::_Sink_iter<char> std::__format::__do_vformat_to<std::__format::_Sink_iter<char>, char, std::basic_format_context<std::__format::_Sink_iter<char>, char>>(std::__format::_Sink_iter<char>, std::basic_string_view<char, std::char_traits<char>>, std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char>> const&, std::locale const*)::'lambda'(std::__format::_Sink_iter<char>&), std::basic_format_context<std::__format::_Sink_iter<char>, char>>(std::__format::_Sink_iter<char>&&, std::basic_format_arg<char>)(int64_t arg1, char arg2)
{
    void arg_8;
    return decltype(auto) std::basic_format_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char>>::_M_visit<std::__format::_Sink_iter<char> std::__format::__do_vformat_to<std::__format::_Sink_iter<char>, char, std::basic_format_context<std::__format::_Sink_iter<char>, char>>(std::__format::_Sink_iter<char>, std::basic_string_view<char, std::char_traits<char>>, std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char>> const&, std::locale const*)::'lambda'(std::__format::_Sink_iter<char>&)>(std::__format::_Sink_iter<char>&&, std::__format::_Arg_t)(
        &arg_8, 
        std::__format::_Sink_iter<char>&& std::forward<std::__format::_Sink_iter<char> std::__format::__do_vformat_to<std::__format::_Sink_iter<char>, char, std::basic_format_context<std::__format::_Sink_iter<char>, char>>(std::__format::_Sink_iter<char>, std::basic_string_view<char, std::char_traits<char>>, std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char>> const&, std::locale const*)::'lambda'(std::__format::_Sink_iter<char>&)>(std::remove_reference<std::__format::_Sink_iter<char>>::type&)(
            arg1), 
        arg2);
}

int64_t std::basic_format_context<std::__format::_Sink_iter<char>, char>::~basic_format_context()
{
    return std::__format::_Optional_locale::~_Optional_locale();
}

std::__format::_Sink_iter<char> std::__format::__do_vformat_to<std::__format::_Sink_iter<char>, char, std::basic_format_context<std::__format::_Sink_iter<char>, char> >(std::__format::_Sink_iter<char> arg1, std::basic_string_view<char> arg2, std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char> > const& arg3, std::locale const* arg4)
{
    std::__format::_Sink_iter<char>* result_2 = arg1;
    int64_t var_248 = arg2;
    void* fsbase;
    int64_t rax_2 = *(fsbase + 0x28);
    std::__format::_Sink_iter<char>* var_1e8 =
        *std::move<std::__format::_Sink_iter<char>&>(&result_2);
    int64_t var_218 = -1;
    void** var_158;
    int64_t* var_210 = &var_158;
    int64_t* var_198;
    std::span<char, 18446744073709551615ul>::span<256ul>(&var_198);
    int64_t* var_1c8 = var_198;
    *var_210 = &data_411200;
    var_210[1] = var_1c8;
    int64_t var_190;
    var_210[2] = var_190;
    char* rax_9;
    char rsi_2;
    rax_9 = std::span<char, 18446744073709551615ul>::begin();
    var_210[3] = rax_9;
    *var_210 = &data_4111d0;
    var_158 = &data_411150;
    std::__format::_Sink_iter<char>* var_38 =
        *std::move<std::__format::_Sink_iter<char>&>(&var_1e8);
    int64_t var_30 = var_218;
    int64_t var_28 = 0;
    std::__format::_Sink_iter<char>* result_1 = nullptr;
    result_1 = result_2;
    char rax_20;
    
    if (std::basic_string_view<char>::size() != 2)
        rax_20 = 0;
    else
    {
        rsi_2 = 0;
        
        if (*std::basic_string_view<char>::operator[](&var_248) != 0x7b)
            rax_20 = 0;
        else
        {
            rsi_2 = 1;
            
            if (*std::basic_string_view<char>::operator[](&var_248) != 0x7d)
                rax_20 = 0;
            else
                rax_20 = 1;
        }
    }
    
    if (rax_20)
    {
        var_1c8 = 0;
        int64_t var_1f8_1 = 0;
        std::__format::_Sink_iter<char>** var_1f0_1 = &var_1e8;
        char var_1d8_1 = 0;
        int64_t var_1e0;
        
        if (0 < (*arg4 & 0xf))
        {
            char rax_27;
            rax_27 = std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char> >::_M_type(
                arg4);
            var_1d8_1 = rax_27;
            int64_t* rax_30 = *(arg4 + 8) + (var_1f8_1 << 4);
            int64_t rdx_6 = rax_30[1];
            var_1e8 = *rax_30;
            var_1e0 = rdx_6;
        }
        else if (!(*arg4 & 0xf) && 0 < *arg4 >> 4)
        {
            std::__format::_Sink_iter<char>** rcx_1 = *(arg4 + 8);
            int64_t rdx_7 = rcx_1[1];
            var_1e8 = *rcx_1;
            var_1e0 = rdx_7;
            var_1d8_1 = rcx_1[2];
        }
        
        var_198 = &result_1;
        int64_t* var_190_1 = &var_1c8;
        std::__format::_Sink_iter<char>* var_278_1 = var_1e8;
        int64_t var_270_1 = var_1e0;
        int64_t var_268_1 = var_1d8_1;
        int64_t var_1d0;
        int64_t var_260_1 = var_1d0;
        decltype(auto) std::visit_format_arg<std::__format::_Sink_iter<char> std::__format::__do_vformat_to<std::__format::_Sink_iter<char>, char, std::basic_format_context<std::__format::_Sink_iter<char>, char>>(std::__format::_Sink_iter<char>, std::basic_string_view<char, std::char_traits<char>>, std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char>> const&, std::locale const*)::'lambda'(std::__format::_Sink_iter<char>&), std::basic_format_context<std::__format::_Sink_iter<char>, char>>(std::__format::_Sink_iter<char>&&, std::basic_format_arg<char>)(
            &var_198, rsi_2);
    }
    
    std::__format::_Sink_iter<char>* result;
    
    if (!rax_20 || !var_1c8)
    {
        int64_t r8;
        
        if (r8)
        {
            int64_t* rax_46 = arg4;
            std::basic_format_context<std::__format::_Sink_iter<char>, char>::basic_format_context(
                &var_1c8, *rax_46, rax_46[1]);
        }
        else
        {
            *(arg4 + 8);
            std::basic_format_context<std::__format::_Sink_iter<char>, char>::basic_format_context(
                &var_1c8, *arg4);
        }
        
        std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_Formatting_scanner(
            &var_198, &var_1c8);
        std::__format::_Scanner<char>::_M_scan();
        result = std::basic_format_context<std::__format::_Sink_iter<char>, char>::out();
        std::basic_format_context<std::__format::_Sink_iter<char>, char>::~basic_format_context();
    }
    else
        result = result_1;
    
    *(fsbase + 0x28);
    
    if (rax_2 == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

void sub_40cb17(void* arg1 @ rbp) __noreturn
{
    std::basic_format_context<std::__format::_Sink_iter<char>, char>::~basic_format_context();
    void* fsbase;
    
    if (*(arg1 - 0x18) != *(fsbase + 0x28))
    {
        __stack_chk_fail();
        /* no return */
    }
    
    _Unwind_Resume();
    __stack_chk_fail();
    /* no return */
}

int64_t std::span<char, 18446744073709551615ul>::empty()
{
    int64_t result;
    result = !std::span<char, 18446744073709551615ul>::size();
    return result;
}

int64_t std::__format::_Seq_sink<std::string>::_M_overflow()
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    char* var_38 = std::span<char, 18446744073709551615ul>::begin();
    void* rdi;
    operator-<char*, std::span<char, 18446744073709551615ul> >(rdi + 0x18, &var_38);
    char* rax_8;
    int64_t rdx_1;
    rax_8 = std::span<char, 18446744073709551615ul>::first(rdi + 8);
    var_38 = rax_8;
    int64_t var_30 = rdx_1;
    
    if (!std::span<char, 18446744073709551615ul>::empty())
    {
        std::span<char, 18446744073709551615ul>::size();
        std::string::append(rdi + 0x120, std::span<char, 18446744073709551615ul>::data());
        *(rdi + 0x18) = std::span<char, 18446744073709551615ul>::begin();
    }
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

char* std::__to_address<char>(char* arg1) __pure
{
    return arg1;
}

int64_t std::string::data()
{
    return std::string::_M_data();
}

uint64_t std::span<char, 18446744073709551615ul>::span<std::string&>(std::string&&& arg1)
{
    *arg1 = std::ranges::__access::_Data::operator()<std::string&>(&std::ranges::_Cpo::data);
    std::ranges::__access::_Size::operator()<std::string&>(&std::ranges::_Cpo::size);
    return std::__extent_storage<18446744073709551615ul>::__extent_storage(&arg1[1]);
}

uint64_t const& std::min<uint64_t>(uint64_t const& arg1, uint64_t const& arg2)
{
    if (*arg2 >= *arg1)
        return arg1;
    
    return arg2;
}

uint64_t std::basic_string_view<char>::compare(std::basic_string_view<char> arg1)
{
    uint64_t* var_20 = arg1;
    uint64_t rsi;
    uint64_t var_38 = rsi;
    uint64_t rax_4 = *std::min<uint64_t>(var_20, &var_38);
    char* rdx;
    int32_t var_14 = std::char_traits<char>::compare(var_20[1], rdx, rax_4);
    
    if (!var_14)
        var_14 = std::basic_string_view<char>::_S_compare(*var_20, var_38);
    
    return var_14;
}

std::chrono::duration<int64_t, std::ratio<1l, 1000000l> > std::chrono::__duration_cast_impl<std::chrono::duration<int64_t, std::ratio<1l, 1000000l> >, std::ratio<1l, 1000l>, int64_t, true, false>::__cast<int64_t, std::ratio<1l, 1000000000l> >(std::chrono::duration<int64_t, std::ratio<1l, 1000000000l> > const& arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_20 = std::chrono::duration<int64_t, std::ratio<1l, 1000000000l> >::count() / 0x3e8;
    int64_t result;
    std::chrono::duration<int64_t, std::ratio<1l, 1000000l> >::duration<int64_t, void>(&result);
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

int64_t* std::__format::__formatter_str<char>::parse(std::basic_format_parse_context<char>&)::'lambda'()::operator()()
{
    int64_t* rdi;
    int64_t* rdx = rdi[1];
    int64_t* result = *rdi;
    *result = *rdx;
    result[1] = rdx[1];
    return result;
}

int64_t std::__format::__formatter_str<char>::parse(std::basic_format_parse_context<char>&)::'lambda0'()::operator()()
{
    int64_t* rdi;
    
    if (**rdi != *rdi[1] && ***rdi != 0x7d)
        return 0;
    
    rdi[2];
    std::__format::__formatter_str<char>::parse(std::basic_format_parse_context<char>&)::'lambda'()::operator()();
    return 1;
}

char* std::__format::__formatter_str<char>::parse(std::basic_format_parse_context<char>& arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    char* result_1 = std::basic_format_parse_context<char>::begin();
    std::basic_format_parse_context<char>* var_50 = std::basic_format_parse_context<char>::end();
    int64_t var_44 = 0;
    int32_t var_3c = 0;
    int32_t var_3c_1 = 0x20;
    std::basic_format_parse_context<char>& var_38 = arg1;
    int64_t* var_30 = &var_44;
    char** var_28 = &result_1;
    int64_t* var_20 = &var_50;
    std::basic_format_parse_context<char>&* var_18 = &var_38;
    char* result;
    
    if (!std::__format::__formatter_str<char>::parse(std::basic_format_parse_context<char>&)::'lambda0'()::operator()())
    {
        result_1 = std::__format::_Spec<char>::_M_parse_fill_and_align(&var_44, result_1);
        
        if (!std::__format::__formatter_str<char>::parse(std::basic_format_parse_context<char>&)::'lambda0'()::operator()())
        {
            result_1 = std::__format::_Spec<char>::_M_parse_width(&var_44, result_1, var_50);
            
            if (!std::__format::__formatter_str<char>::parse(std::basic_format_parse_context<char>&)::'lambda0'()::operator()())
            {
                result_1 =
                    std::__format::_Spec<char>::_M_parse_precision(&var_44, result_1, var_50);
                
                if (!std::__format::__formatter_str<char>::parse(std::basic_format_parse_context<char>&)::'lambda0'()::operator()())
                {
                    if (*result_1 == 0x73)
                        result_1 = &result_1[1];
                    
                    if (!std::__format::__formatter_str<char>::parse(std::basic_format_parse_context<char>&)::'lambda0'()::operator()())
                    {
                        *(fsbase + 0x28);
                        
                        if (rax != *(fsbase + 0x28))
                        {
                            __stack_chk_fail();
                            /* no return */
                        }
                        
                        std::__format::__failed_to_parse_format_spec();
                        /* no return */
                    }
                    
                    result = result_1;
                }
                else
                    result = result_1;
            }
            else
                result = result_1;
        }
        else
            result = result_1;
    }
    else
        result = result_1;
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::println<>(_IO_FILE* arg1, std::basic_format_string<char, std::type_identity<char>::type> arg2, char&& arg3)
{
    char&& var_70 = arg3;
    void* fsbase;
    int64_t rax_2 = *(fsbase + 0x28);
    void var_58;
    std::format<>(&var_58, arg2);
    std::print<std::string>(arg1, 3, &data_4019a1);
    std::string::~string();
    
    if (rax_2 == *(fsbase + 0x28))
        return rax_2 - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

void sub_40d107(void* arg1 @ rbp) __noreturn
{
    std::string::~string();
    void* fsbase;
    
    if (*(arg1 - 0x28) != *(fsbase + 0x28))
    {
        __stack_chk_fail();
        /* no return */
    }
    
    _Unwind_Resume();
    __stack_chk_fail();
    /* no return */
}

int64_t* Timestamped<canzero_frame>::Timestamped()
{
    int64_t* result;
    *result = 0;
    result[1] = 0;
    result[2] = 0;
    return result;
}

int64_t std::println<uint16_t>(_IO_FILE* arg1, std::basic_format_string<char, std::type_identity<uint16_t>::type> arg2, uint16_t&& arg3)
{
    uint16_t&& var_70 = arg3;
    void* fsbase;
    int64_t rax_2 = *(fsbase + 0x28);
    void var_58;
    std::format<uint16_t>(&var_58, arg2);
    std::print<std::string>(arg1, 3, &data_4019a1);
    std::string::~string();
    
    if (rax_2 == *(fsbase + 0x28))
        return rax_2 - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

void sub_40d227(void* arg1 @ rbp) __noreturn
{
    std::string::~string();
    void* fsbase;
    
    if (*(arg1 - 0x28) != *(fsbase + 0x28))
    {
        __stack_chk_fail();
        /* no return */
    }
    
    _Unwind_Resume();
    __stack_chk_fail();
    /* no return */
}

std::remove_reference<int32_t>::type& std::forward<int32_t>(std::remove_reference<int32_t>::type& arg1) __pure
{
    return arg1;
}

int32_t&& std::pair<uint16_t, char const*>::pair<int32_t, char const*>(int32_t&& arg1, char const*&& arg2)
{
    *arg1 = *std::forward<int32_t>(arg2);
    std::remove_reference<char const*>::type* rdx;
    *(arg1 + 8) = *std::forward<char const*>(rdx);
    return arg1;
}

uint16_t const& std::pair<uint16_t, char const*>::pair(uint16_t const& arg1, char const* const& arg2)
{
    *arg1 = *arg2;
    int64_t* rdx;
    *(arg1 + 8) = *rdx;
    return arg1;
}

std::pair<uint16_t, char const*> std::__format::__parse_integer<char>(char const* arg1, char const* arg2)
{
    char const* var_50 = arg1;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    char const* rax_1 = var_50;
    int16_t var_38 = 0;
    char rax_4;
    
    if (!std::__from_chars_alnum<true, uint16_t>(&var_50, arg2, &var_38, 0xa) || rax_1 == var_50)
        rax_4 = 0;
    else
        rax_4 = 1;
    
    int32_t rax_5;
    int64_t var_28;
    
    if (!rax_4)
    {
        int32_t var_3c = 0;
        var_38 = 0;
        std::pair<uint16_t, char const*>::pair<int32_t, std::nullptr_t>(&var_28, &var_3c);
        rax_5 = var_28;
    }
    else
    {
        std::pair<uint16_t, char const*>::pair<uint16_t&, char const*&>(&var_28, &var_38);
        rax_5 = var_28;
    }
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return rax_5;
    
    __stack_chk_fail();
    /* no return */
}

std::remove_reference<std::nullptr_t>::type& std::forward<std::nullptr_t>(std::remove_reference<std::nullptr_t>::type& arg1) __pure
{
    return arg1;
}

int32_t&& std::pair<uint16_t, char const*>::pair<int32_t, std::nullptr_t>(int32_t&& arg1, std::nullptr_t&& arg2)
{
    *arg1 = *std::forward<int32_t>(arg2);
    *(arg1 + 8) = 0;
    return arg1;
}

int64_t std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1>::begin() const(int64_t* arg1)
{
    return *arg1;
}

int64_t std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1>::end() const(void* arg1)
{
    return *(arg1 + 8);
}

char*** std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1>::subrange<char const*>(char const*, char const*)(char*** arg1, char** arg2, int64_t arg3)
{
    char const*& var_28 = arg2;
    *arg1 = *std::move<char const*&>(&var_28);
    arg1[1] = arg3;
    char*** var_18 = arg1;
    int64_t var_10 = 0;
    return arg1;
}

int64_t std::__unicode::_Utf_view<char32_t, std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1> >::_Utf_view(std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1>&&)(int64_t* arg1, int64_t arg2)
{
    int64_t* rax_1 = std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1>&& std::forward<std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1>>(std::remove_reference<std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1>>::type&)(
        arg2);
    int64_t rdx = rax_1[1];
    int64_t result = *rax_1;
    *arg1 = result;
    arg1[1] = rdx;
    return result;
}

int64_t std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_curr() __pure
{
    int64_t rdi;
    return rdi + 0x10;
}

int32_t* std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_Utf_iterator(char const* arg1, char const* arg2, char const* arg3)
{
    std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_First_and_curr<char const*>::_First_and_curr(
        &arg1[8], arg2);
    arg1[0x18] = 0;
    arg1[0x19] = 0;
    arg1[0x1a] = 0;
    int64_t rcx;
    *(arg1 + 0x20) = rcx;
    int64_t rax_9;
    rax_9 = *std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_curr()
        != *(arg1 + 0x20);
    
    if (rax_9)
        return std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_read();
    
    int32_t* result = arg1;
    *result = 0;
    return result;
}

int64_t* std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::operator++(int32_t arg1)
{
    int64_t* result = arg1;
    int32_t rdx;
    int32_t var_2c = rdx;
    int64_t* rsi;
    int64_t rbx = rsi[1];
    *result = *rsi;
    result[1] = rbx;
    int64_t rbx_1 = rsi[3];
    result[2] = rsi[2];
    result[3] = rbx_1;
    result[4] = rsi[4];
    std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::operator++();
    return result;
}

uint64_t std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::operator*()
{
    uint64_t rdi;
    *(rdi + 0x18);
    return *std::array<char32_t, 1ul>::operator[](rdi);
}

int64_t std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::base()
{
    return std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_curr();
}

int64_t std::__format::_Spec<char>::_S_align(char arg1) __pure
{
    int32_t rax_1 = arg1;
    
    if (rax_1 == 0x5e)
        return 3;
    
    if (rax_1 == 0x3c)
        return 1;
    
    if (rax_1 == 0x3e)
        return 2;
    
    return 0;
}

int64_t std::__format::_Spec<char>::_S_sign(char arg1) __pure
{
    int32_t rax_1 = arg1;
    
    if (rax_1 == 0x2d)
        return 2;
    
    if (rax_1 == 0x20)
        return 3;
    
    if (rax_1 == 0x2b)
        return 1;
    
    return 0;
}

char const* std::__format::_Spec<char>::_S_parse_width_or_precision(char const* arg1, char const* arg2, uint16_t& arg3, bool& arg4, std::basic_format_parse_context<char>& arg5)
{
    char const* result = arg1;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int32_t var_28;
    
    if (std::__format::__is_digit(*result))
    {
        int32_t rax_6;
        int64_t rdx_1;
        rax_6 = std::__format::__parse_integer<char>(result, arg2);
        var_28 = rax_6;
        int64_t var_20_1 = rdx_1;
        std::remove_reference<uint16_t>::type* rax_8 =
            std::get<0ul, uint16_t, char const*>(&var_28);
        std::remove_reference<char const*>::type* rax_9 =
            std::get<1ul, uint16_t, char const*>(&var_28);
        
        if (!*rax_9)
        {
            *(fsbase + 0x28);
            
            if (rax != *(fsbase + 0x28))
            {
                __stack_chk_fail();
                /* no return */
            }
            
            std::__throw_format_error("format error: invalid width or p…");
            /* no return */
        }
        
        result = *rax_9;
        *arg3 = *rax_8;
    }
    else if (*result == 0x7b)
    {
        *arg4 = true;
        void* var_50 = &result[1];
        
        if (var_50 == arg2)
        {
            *(fsbase + 0x28);
            
            if (rax != *(fsbase + 0x28))
            {
                __stack_chk_fail();
                /* no return */
            }
            
            std::__format::__unmatched_left_brace_in_format_string();
            /* no return */
        }
        
        if (*var_50 != 0x7d)
        {
            int32_t rax_30;
            int64_t rdx_5;
            rax_30 = std::__format::__parse_arg_id<char>(var_50, arg2);
            var_28 = rax_30;
            int64_t var_20_2 = rdx_5;
            uint16_t* var_48_1 = std::get<0ul, uint16_t, char const*>(&var_28);
            std::remove_reference<char const*>::type* rax_33 =
                std::get<1ul, uint16_t, char const*>(&var_28);
            
            if (!*rax_33 || arg2 == *rax_33 || **rax_33 != 0x7d)
            {
                *(fsbase + 0x28);
                
                if (rax != *(fsbase + 0x28))
                {
                    __stack_chk_fail();
                    /* no return */
                }
                
                std::__format::__invalid_arg_id_in_format_string();
                /* no return */
            }
            
            var_50 = *rax_33;
            *var_48_1;
            std::basic_format_parse_context<char>::check_arg_id(arg5);
            *arg3 = *var_48_1;
        }
        else
            *arg3 = std::basic_format_parse_context<char>::next_arg_id();
        
        result = var_50 + 1;
    }
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

void std::_Optional_payload_base<telemetry_board::TcpConnection>::_Storage<telemetry_board::TcpConnection, false>::~_Storage() __pure
{
    int64_t rdi;
    int64_t var_10 = rdi;
}

void* std::_Optional_payload_base<telemetry_board::TcpConnection>::_M_reset()
{
    void* result;
    
    if (*(result + 8))
        return std::_Optional_payload_base<telemetry_board::TcpConnection>::_M_destroy();
    
    *(result + 8) = 0;
    return result;
}

uint64_t std::_Optional_base_impl<telemetry_board::TcpConnection, std::_Optional_base<telemetry_board::TcpConnection, false, false> >::_M_is_engaged()
{
    void* rdi;
    return *(rdi + 8);
}

int64_t std::_Optional_base_impl<telemetry_board::TcpConnection, std::_Optional_base<telemetry_board::TcpConnection, false, false> >::_M_get()
{
    return std::_Optional_payload_base<telemetry_board::TcpConnection>::_M_get();
}

void* std::optional<Timestamped<canzero_frame> >::optional(std::nullopt_t arg1)
{
    return std::_Optional_base<Timestamped<canzero_frame>, false, false>::_Optional_base();
}

void* std::optional<Timestamped<canzero_frame> >::optional<Timestamped<canzero_frame>, true>(Timestamped<canzero_frame>&& arg1)
{
    std::remove_reference<Timestamped<canzero_frame> >::type* rsi;
    return std::_Optional_base<Timestamped<canzero_frame>, false, false>::_Optional_base<Timestamped<canzero_frame>, false>(
        arg1, std::forward<Timestamped<canzero_frame> >(rsi));
}

std::_Optional_payload<Timestamped<canzero_frame>, true, false, false>&&& std::move<std::_Optional_payload<Timestamped<canzero_frame>, true, false, false>&>(std::_Optional_payload<Timestamped<canzero_frame>, true, false, false>&&& arg1) __pure
{
    return arg1;
}

Timestamped<canzero_frame>&& Timestamped<canzero_frame>::operator=(Timestamped<canzero_frame>&& arg1)
{
    int64_t* rsi;
    int64_t rdx = rsi[1];
    *arg1 = *rsi;
    *(arg1 + 8) = rdx;
    Timestamp::operator=(arg1 + 0x10);
    return arg1;
}

Timestamped<canzero_frame>* std::_Optional_payload_base<Timestamped<canzero_frame> >::_M_move_assign(std::_Optional_payload_base<Timestamped<canzero_frame> >&& arg1)
{
    void* rsi;
    
    if (*(arg1 + 0x14) && *(rsi + 0x14))
        return Timestamped<canzero_frame>::operator=(
            std::_Optional_payload_base<Timestamped<canzero_frame> >::_M_get());
    
    if (!*(rsi + 0x14))
        return std::_Optional_payload_base<Timestamped<canzero_frame> >::_M_reset();
    
    return std::_Optional_payload_base<Timestamped<canzero_frame> >::_M_construct<Timestamped<canzero_frame> >(
        arg1);
}

int64_t std::_Optional_base_impl<Timestamped<canzero_frame>, std::_Optional_base<Timestamped<canzero_frame>, false, false> >::_M_get()
{
    return std::_Optional_payload_base<Timestamped<canzero_frame> >::_M_get();
}

Timestamped<canzero_frame>& std::__addressof<Timestamped<canzero_frame> >(Timestamped<canzero_frame>& arg1) __pure
{
    return arg1;
}

void std::_Optional_payload_base<canzero_frame>::_Storage<canzero_frame, true>::_Storage() __pure
{
    int64_t rdi;
    int64_t var_10 = rdi;
}

void* std::_Optional_payload_base<canzero_frame>::_Optional_payload_base()
{
    void* result;
    *(result + 0x10) = 0;
    return result;
}

void* std::_Optional_payload<canzero_frame, true, true, true>::_Optional_payload()
{
    return std::_Optional_payload_base<canzero_frame>::_Optional_payload_base();
}

void* std::_Optional_base<canzero_frame, true, true>::_Optional_base()
{
    return std::_Optional_payload<canzero_frame, true, true, true>::_Optional_payload();
}

void* std::optional<canzero_frame>::optional(std::nullopt_t arg1)
{
    return std::_Optional_base<canzero_frame, true, true>::_Optional_base();
}

void* std::optional<canzero_frame>::optional<canzero_frame, true>(canzero_frame&& arg1)
{
    std::remove_reference<canzero_frame>::type* rsi;
    return std::_Optional_base<canzero_frame, true, true>::_Optional_base<canzero_frame, false>(
        arg1, std::forward<canzero_frame>(rsi));
}

uint64_t std::_Optional_base_impl<canzero_frame, std::_Optional_base<canzero_frame, true, true> >::_M_is_engaged()
{
    void* rdi;
    return *(rdi + 0x10);
}

int64_t std::_Optional_base_impl<canzero_frame, std::_Optional_base<canzero_frame, true, true> >::_M_get()
{
    return std::_Optional_payload_base<canzero_frame>::_M_get();
}

int32_t* Timestamped<canzero_frame>::Timestamped(canzero_frame arg1, Timestamp arg2)
{
    int64_t* var_10 = arg1;
    *var_10 = arg2;
    int64_t rdx;
    var_10[1] = rdx;
    return Timestamp::Timestamp(&var_10[2]);
}

std::remove_reference<char&>::type& std::forward<char&>(std::remove_reference<char&>::type& arg1) __pure
{
    return arg1;
}

int64_t std::string::_M_destroy(uint64_t arg1)
{
    void* rax_2 = std::string::_M_data();
    int64_t rsi;
    int64_t var_20 = rsi + 1;
    return std::__new_allocator<char>::deallocate(std::string::_M_get_allocator(), rax_2);
}

int64_t std::string::capacity()
{
    void* rdi;
    
    if (!std::string::_M_is_local())
        return *(rdi + 0x10);
    return 0xf;
}

uint64_t std::string::reserve(uint64_t arg1)
{
    uint64_t result_2;
    uint64_t result_1 = result_2;
    int64_t rax_1 = std::string::capacity();
    uint64_t result = result_1;
    
    if (rax_1 >= result)
        return result;
    
    char* rax_3 = std::string::_M_create(arg1, &result_1);
    uint64_t rbx_1 = std::string::length() + 1;
    std::string::_S_copy(rax_3, std::string::_M_data(), rbx_1);
    std::string::_M_dispose();
    std::string::_M_data(arg1);
    return std::string::_M_capacity(arg1);
}

char* std::__ptr_traits_ptr_to<char const*, char const, false>::pointer_to(char const& arg1)
{
    return std::addressof<char const>(arg1);
}

int64_t std::string::_M_create(uint64_t& arg1, uint64_t arg2)
{
    int64_t rbx = *arg2;
    std::string::max_size();
    
    if (0x7fffffffffffffff < rbx)
    {
        std::__throw_length_error("basic_string::_M_create");
        /* no return */
    }
    
    int64_t rdx;
    
    if (rdx < *arg2 && *arg2 < rdx * 2)
    {
        *arg2 = rdx * 2;
        int64_t rbx_1 = *arg2;
        std::string::max_size();
        
        if (0x7fffffffffffffff < rbx_1)
        {
            std::string::max_size();
            *arg2 = 0x7fffffffffffffff;
        }
    }
    
    return std::string::_S_allocate(std::string::_M_get_allocator(), *arg2 + 1);
}

int32_t std::__bit_width<uint64_t>(uint64_t arg1)
{
    int32_t var_c = 0x40;
    return 0x40 - std::__countl_zero<uint64_t>(arg1);
}

int32_t std::__bit_width<uint128_t>(uint128_t arg1)
{
    int32_t var_c = 0x80;
    int64_t rdi;
    return 0x80 - std::__countl_zero<uint128_t>(rdi);
}

int64_t std::__to_chars_10_impl<uint128_t>(char* arg1, uint32_t arg2, uint128_t arg3)
{
    int64_t rdx;
    int64_t var_138 = rdx;
    int64_t rcx;
    int64_t var_130 = rcx;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_e8;
    __builtin_strcpy(&var_e8, 
        "00010203040506070809101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899");
    int32_t var_10c = arg2 - 1;
    
    while (true)
    {
        bool c_3 = /* bool c_3 = unimplemented  {sbb rax, qword [rbp-0x128]} */;
        
        if (!c_3)
            break;
        
        int64_t rax_5;
        int64_t rdx_2;
        rax_5 = __umodti3(var_138, var_130, 0x64, 0);
        int64_t rax_6 = rax_5 * 2;
        int64_t var_f0_1 = rdx_2 + rdx_2;
        int64_t rax_9;
        int64_t rdx_5;
        rax_9 = __udivti3(var_138, var_130, 0x64, 0);
        var_138 = rax_9;
        var_130 = rdx_5;
        arg1[var_10c] = *(rax_6 + 1 + &var_e8);
        arg1[var_10c - 1] = *(rax_6 + &var_e8);
        var_10c -= 2;
    }
    
    bool c_5 = /* bool c_5 = unimplemented  {sbb rax, qword [rbp-0x128]} */;
    
    if (!c_5)
        *arg1 = var_138 + 0x30;
    else
    {
        int64_t rax_23 = var_138 * 2;
        int64_t var_100_1 = var_130 + var_130;
        arg1[1] = *(rax_23 + 1 + &var_e8);
        *arg1 = *(rax_23 + &var_e8);
    }
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

bool __ops::_Val_less_iter::operator()<char32_t const, char32_t const*>(char32_t const& arg1, char32_t const* arg2)
{
    char32_t const& var_10 = arg1;
    int32_t rax_2;
    int32_t* rdx;
    rax_2 = *arg2 < *rdx;
    return rax_2;
}

bool __ops::_Iter_less_val::operator()<uint32_t const*, uint32_t const>(uint32_t const* arg1, uint32_t const& arg2)
{
    uint32_t const* var_10 = arg1;
    int32_t rax_2;
    int32_t* rdx;
    rax_2 = *arg2 < *rdx;
    return rax_2;
}

char32_t const& __ops::_Iter_equals_val<char32_t const>::_Iter_equals_val(char32_t const& arg1)
{
    int64_t rsi;
    *arg1 = rsi;
    return arg1;
}

char32_t const* std::__find_if<char32_t const*, __ops::_Iter_equals_val<char32_t const> >(char32_t const* arg1, char32_t const* arg2, __gnu_cxx::__ops::_Iter_equals_val<char32_t const> arg3, std::random_access_iterator_tag arg4)
{
    char32_t const* var_20 = arg1;
    int64_t var_30 = arg3;
    
    for (int64_t i = (arg2 - var_20) >> 4; i > 0; i -= 1)
    {
        if (__ops::_Iter_equals_val<char32_t const>::operator()<char32_t const*>(&var_30))
            return var_20;
        
        void* var_20_1 = &var_20[1];
        
        if (__ops::_Iter_equals_val<char32_t const>::operator()<char32_t const*>(&var_30))
            return var_20_1;
        
        void* var_20_2 = var_20_1 + 4;
        
        if (__ops::_Iter_equals_val<char32_t const>::operator()<char32_t const*>(&var_30))
            return var_20_2;
        
        char32_t* var_20_3 = var_20_2 + 4;
        
        if (__ops::_Iter_equals_val<char32_t const>::operator()<char32_t const*>(&var_30))
            return var_20_3;
        
        var_20 = &var_20_3[1];
    }
    
    int64_t rax_11 = (arg2 - var_20) >> 2;
    
    if (rax_11 == 3)
    {
        if (__ops::_Iter_equals_val<char32_t const>::operator()<char32_t const*>(&var_30))
            return var_20;
        
        var_20 = &var_20[1];
        goto label_40e597;
    }
    
    if (rax_11 == 1)
    {
        label_40e5b9:
        
        if (__ops::_Iter_equals_val<char32_t const>::operator()<char32_t const*>(&var_30))
            return var_20;
        
        void* var_20_4 = &var_20[1];
    }
    else if (rax_11 == 2)
    {
        label_40e597:
        
        if (__ops::_Iter_equals_val<char32_t const>::operator()<char32_t const*>(&var_30))
            return var_20;
        
        var_20 = &var_20[1];
        goto label_40e5b9;
    }
    
    return arg2;
}

uint64_t std::span<char, 18446744073709551615ul>::span<256ul>(char[256ul]& arg1)
{
    int64_t rsi;
    *arg1 = rsi;
    return std::__extent_storage<18446744073709551615ul>::__extent_storage(arg1 + 8);
}

std::__format::_Sink<char>* std::addressof<std::__format::_Sink<char> >(std::__format::_Sink<char>& arg1)
{
    return std::__addressof<std::__format::_Sink<char> >(arg1);
}

int64_t std::__format::_Sink_iter<char>&& std::forward<std::__format::_Sink_iter<char> std::__format::__do_vformat_to<std::__format::_Sink_iter<char>, char, std::basic_format_context<std::__format::_Sink_iter<char>, char>>(std::__format::_Sink_iter<char>, std::basic_string_view<char, std::char_traits<char>>, std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char>> const&, std::locale const*)::'lambda'(std::__format::_Sink_iter<char>&)>(std::remove_reference<std::__format::_Sink_iter<char>>::type&)(int64_t arg1) __pure
{
    return arg1;
}

uint64_t std::__format::_Sink<char>::_M_reserve(uint64_t arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    char* var_38 = std::span<char, 18446744073709551615ul>::begin();
    char* rax_8;
    int64_t rdx_2;
    rax_8 = std::span<char, 18446744073709551615ul>::subspan(arg1 + 8, 
        operator-<char*, std::span<char, 18446744073709551615ul> >(arg1 + 0x18, &var_38));
    var_38 = rax_8;
    int64_t var_30 = rdx_2;
    int64_t rax_9;
    int64_t rsi;
    rax_9 = std::span<char, 18446744073709551615ul>::size() >= rsi;
    uint64_t result;
    
    if (!rax_9)
    {
        int64_t rax_12;
        rax_12 = std::span<char, 18446744073709551615ul>::size() >= rsi;
        int64_t rax_24;
        
        if (rax_12)
        {
            (**arg1)(arg1);
            var_38 = std::span<char, 18446744073709551615ul>::begin();
            char* rax_23;
            int64_t rdx_6;
            rax_23 = std::span<char, 18446744073709551615ul>::subspan(arg1 + 8, 
                operator-<char*, std::span<char, 18446744073709551615ul> >(arg1 + 0x18, &var_38));
            var_38 = rax_23;
            int64_t var_30_1 = rdx_6;
            rax_24 = std::span<char, 18446744073709551615ul>::size() >= rsi;
        }
        
        if (!rax_12 || !rax_24)
            result = 0;
        else
            result = arg1;
    }
    else
        result = arg1;
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::__format::_Sink<char>::_Reservation::operator bool()
{
    int64_t result;
    int64_t* rdi;
    result = *rdi;
    return result;
}

int64_t std::__format::_Sink<char>::_Reservation::get()
{
    int64_t* rdi;
    *rdi;
    return __normal_iterator<char*, std::span<char, 18446744073709551615ul> >::operator->();
}

int64_t std::__format::_Sink<char>::_Reservation::_M_bump(uint64_t arg1)
{
    int64_t rsi;
    return (*(**arg1 + 0x10))(*arg1, rsi);
}

void* std::basic_format_context<std::__format::_Sink_iter<char>, char>::basic_format_context(std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char> > arg1, std::__format::_Sink_iter<char> arg2)
{
    int64_t* var_20 = arg1;
    int64_t rcx;
    int64_t var_28 = rcx;
    *var_20 = arg2;
    int64_t rdx;
    var_20[1] = rdx;
    var_20[2] = *std::move<std::__format::_Sink_iter<char>&>(&var_28);
    var_20[3] = 0;
    var_20[4] = 0;
    return &var_20[3];
}

std::locale* std::basic_format_context<std::__format::_Sink_iter<char>, char>::basic_format_context(std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char> > arg1, std::__format::_Sink_iter<char> arg2, std::locale const& arg3)
{
    int64_t* var_10 = arg1;
    int64_t rcx;
    int64_t var_18 = rcx;
    *var_10 = arg2;
    var_10[1] = arg3;
    var_10[2] = *std::move<std::__format::_Sink_iter<char>&>(&var_18);
    return std::__format::_Optional_locale::_Optional_locale(&var_10[3]);
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>& std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_Formatting_scanner(std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg1, std::basic_string_view<char> arg2)
{
    int64_t rcx;
    int64_t var_20 = rcx;
    uint64_t rdx;
    std::__format::_Scanner<char>::_Scanner(arg1, rdx);
    *arg1 = &data_411130;
    *(arg1 + 0x30) = arg2;
    return arg1;
}

std::__format::_Sink_iter<char>* std::basic_format_context<std::__format::_Sink_iter<char>, char>::out()
{
    void* rdi;
    return *std::move<std::__format::_Sink_iter<char>&>(rdi + 0x10);
}

char* std::span<char, 18446744073709551615ul>::begin()
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    char* result;
    __normal_iterator<char*, std::span<char, 18446744073709551615ul> >::__normal_iterator(&result);
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

__gnu_cxx::__normal_iterator<char*, std::span<char, 18446744073709551615ul> >::difference_type operator-<char*, std::span<char, 18446744073709551615ul> >(__normal_iterator<char*, std::span<char, 18446744073709551615ul> > const& arg1, __normal_iterator<char*, std::span<char, 18446744073709551615ul> > const& arg2)
{
    return *__normal_iterator<char*, std::span<char, 18446744073709551615ul> >::base()
        - *__normal_iterator<char*, std::span<char, 18446744073709551615ul> >::base();
}

int64_t std::span<char, 18446744073709551615ul>::first(uint64_t arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t result;
    std::span<char, 18446744073709551615ul>::span<char*>(&result, 
        std::span<char, 18446744073709551615ul>::data());
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

char* std::string::append(char const* arg1, uint64_t arg2)
{
    char* rdx;
    std::string::_M_check_length(arg1, 0, rdx);
    return std::string::_M_append(arg1, arg2);
}

int64_t std::basic_string_view<char>::substr(uint64_t arg1, uint64_t arg2)
{
    uint64_t rdx;
    uint64_t var_50 = rdx;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    uint64_t rax_4 =
        std::__sv_check(std::basic_string_view<char>::size(), arg2, "basic_string_view::substr");
    int64_t result = *arg1 - rax_4;
    uint64_t var_30 = *std::min<uint64_t>(&var_50, &result);
    std::basic_string_view<char>::basic_string_view(&result, *(arg1 + 8) + rax_4);
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

uint64_t std::basic_string_view<char>::_S_compare(uint64_t arg1, uint64_t arg2) __pure
{
    uint64_t result = arg1 - arg2;
    
    if (result >= 0x80000000)
        return 0x7fffffff;
    
    if (result > -0x80000001)
        return result;
    
    return 0x80000000;
}

int64_t const& std::chrono::duration<int64_t, std::ratio<1l, 1000000l> >::duration<int64_t, void>(int64_t const& arg1)
{
    int64_t* rsi;
    *arg1 = *rsi;
    return arg1;
}

std::string std::format<>(std::basic_format_string<char, std::type_identity<char>::type> arg1, char&& arg2)
{
    int64_t result = arg1;
    void* fsbase;
    int64_t rax_2 = *(fsbase + 0x28);
    int64_t var_28;
    std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char> >::basic_format_args<>(
        &var_28);
    std::vformat(result, arg2);
    *(fsbase + 0x28);
    
    if (rax_2 == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::print<std::string>(_IO_FILE* arg1, std::basic_format_string<char, std::type_identity<std::string>::type> arg2, std::string&& arg3)
{
    void* fsbase;
    int64_t rax_2 = *(fsbase + 0x28);
    int64_t rax_6;
    std::string* rcx;
    int64_t rdx_1;
    rax_6 = std::__format::_Arg_store<std::basic_format_context<std::__format::_Sink_iter<char>, char>, std::basic_string_view<char> >::_S_make_elt<std::string>(
        rcx);
    int64_t var_48 = rax_6;
    int64_t var_40 = rdx_1;
    int64_t var_58;
    std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char> >::basic_format_args<std::basic_string_view<char> >(
        &var_58);
    std::vprint_unicode(arg1, arg2, arg3);
    
    if (rax_2 == *(fsbase + 0x28))
        return rax_2 - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

char const* std::__format::_Spec<char>::_M_parse_precision(char const* arg1, char const* arg2, std::basic_format_parse_context<char>& arg3)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    char const* result;
    
    if (*arg2 == 0x2e)
    {
        char* result_1 = &arg2[1];
        bool var_19 = false;
        std::basic_format_parse_context<char>* rcx;
        
        if (result_1 != arg3)
            result_1 = std::__format::_Spec<char>::_S_parse_width_or_precision(&arg2[1], arg3, 
                &arg1[6], &var_19, rcx);
        
        if (result_1 == &arg2[1])
        {
            *(fsbase + 0x28);
            
            if (rax != *(fsbase + 0x28))
            {
                __stack_chk_fail();
                /* no return */
            }
            
            std::__throw_format_error("format error: missing precision …");
            /* no return */
        }
        
        char rdx_1;
        
        if (!var_19)
            rdx_1 = 1;
        else
            rdx_1 = 2;
        
        arg1[1] = (arg1[1] & 0xf9) | ((rdx_1 & 3) * 2);
        result = result_1;
    }
    else
        result = arg2;
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

std::string std::format<uint16_t>(std::basic_format_string<char, std::type_identity<uint16_t>::type> arg1, uint16_t&& arg2)
{
    int64_t result = arg1;
    void* fsbase;
    int64_t rax_2 = *(fsbase + 0x28);
    int64_t rax_6;
    uint16_t* rcx;
    int64_t rdx_2;
    rax_6 = std::__format::_Arg_store<std::basic_format_context<std::__format::_Sink_iter<char>, char>, uint32_t>::_S_make_elt<uint16_t>(
        rcx);
    int64_t var_48 = rax_6;
    int64_t var_40 = rdx_2;
    int64_t var_58;
    std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char> >::basic_format_args<uint32_t>(
        &var_58);
    std::vformat(result, arg2);
    *(fsbase + 0x28);
    
    if (rax_2 == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

uint8_t std::__from_chars_alnum_to_val<true>(uint8_t arg1) __pure
{
    return arg1 - 0x30;
}

bool std::__from_chars_alnum<true, uint16_t>(char const*& arg1, char const* arg2, uint16_t& arg3, int32_t arg4)
{
    int32_t rax_2 = std::__bit_width<uint32_t>(arg4 & 0x3f);
    int32_t var_10 = 0x10;
    
    while (true)
    {
        if (arg2 == *arg1)
            return 1;
        
        uint8_t rax_7 = std::__from_chars_alnum_to_val<true>(**arg1);
        
        if (arg4 <= rax_7)
            break;
        
        var_10 -= rax_2;
        
        if (var_10 < 0)
        {
            if (std::__raise_and_add<uint16_t>(arg3, arg4, rax_7) != 1)
            {
                char i;
                
                do
                {
                    *arg1 = &(*arg1)[1];
                    uint32_t rax_28;
                    
                    if (arg2 != *arg1)
                        rax_28 = std::__from_chars_alnum_to_val<true>(**arg1);
                    
                    if (arg2 == *arg1 || arg4 <= rax_28)
                        i = 0;
                    else
                        i = 1;
                } while (i);
                return 0;
            }
        }
        else
            *arg3 = arg4 * *arg3 + rax_7;
        
        *arg1 = &(*arg1)[1];
    }
    
    return 1;
}

uint16_t&&& std::pair<uint16_t, char const*>::pair<uint16_t&, char const*&>(uint16_t&&& arg1, char const*&&& arg2)
{
    *arg1 = *std::forward<uint16_t&>(arg2);
    std::remove_reference<char const*&>::type* rdx;
    arg1[1] = *std::forward<char const*&>(rdx);
    return arg1;
}

char const*&&& std::move<char const*&>(char const*&&& arg1) __pure
{
    return arg1;
}

int64_t std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1>&& std::forward<std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1>>(std::remove_reference<std::ranges::subrange<char const*, char const*, (std::ranges::subrange_kind)1>>::type&)(int64_t arg1) __pure
{
    return arg1;
}

char const* std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_First_and_curr<char const*>::_First_and_curr(char const* arg1, char const* arg2)
{
    *arg1 = arg2;
    int64_t rdx;
    *(arg1 + 8) = rdx;
    return arg1;
}

int64_t std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_read()
{
    return std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_read_utf8();
}

void* std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::operator++()
{
    int64_t rdx_1;
    void* result;
    
    if (*(result + 0x18) + 1 == *(result + 0x19))
        rdx_1 = *std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_curr();
    
    char rax_10;
    
    if (*(result + 0x18) + 1 != *(result + 0x19) || rdx_1 == *(result + 0x20))
        rax_10 = 0;
    else
        rax_10 = 1;
    
    if (rax_10)
    {
        char rax_12 = *(result + 0x1a);
        char** rax_14 = std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_curr();
        uint64_t rax_15 = rax_12;
        char** var_20_1 = rax_14;
        std::__advance<char const*, int64_t>(rax_14, rax_15, rax_15);
        int64_t rax_21;
        rax_21 = *std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_curr()
            == *(result + 0x20);
        
        if (!rax_21)
            std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_read();
        else
            *(result + 0x18) = 0;
    }
    else if (*(result + 0x18) + 1 < *(result + 0x19))
        *(result + 0x18) += 1;
    
    return result;
}

int64_t std::array<char32_t, 1ul>::operator[](uint64_t arg1) __pure
{
    int64_t rsi;
    return arg1 + (rsi << 2);
}

int64_t std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_curr()
{
    void* rdi;
    return *(rdi + 0x10);
}

int64_t std::_Optional_payload_base<telemetry_board::TcpConnection>::_M_destroy()
{
    void* rdi;
    *(rdi + 8) = 0;
    return telemetry_board::TcpConnection::~TcpConnection();
}

int64_t std::_Optional_payload_base<telemetry_board::TcpConnection>::_M_get() __pure
{
    int64_t result;
    return result;
}

std::remove_reference<Timestamped<canzero_frame> >::type& std::forward<Timestamped<canzero_frame> >(std::remove_reference<Timestamped<canzero_frame> >::type& arg1) __pure
{
    return arg1;
}

void* std::_Optional_payload<Timestamped<canzero_frame>, true, false, false>::_Optional_payload<Timestamped<canzero_frame> >(std::in_place_t, Timestamped<canzero_frame>&&)(int64_t arg1, Timestamped<canzero_frame>* arg2)
{
    return std::_Optional_payload_base<Timestamped<canzero_frame> >::_Optional_payload_base<Timestamped<canzero_frame> >(
        arg1, arg2);
}

int32_t* Timestamped<canzero_frame>::Timestamped(Timestamped<canzero_frame>&& arg1)
{
    int64_t* rsi;
    int64_t rdx = rsi[1];
    *arg1 = *rsi;
    *(arg1 + 8) = rdx;
    return Timestamp::Timestamp(arg1 + 0x10);
}

int32_t* std::_Optional_payload_base<Timestamped<canzero_frame> >::_Storage<Timestamped<canzero_frame>, true>::_Storage<Timestamped<canzero_frame> >(std::in_place_t arg1, Timestamped<canzero_frame>&& arg2)
{
    return Timestamped<canzero_frame>::Timestamped(arg1);
}

void* std::_Optional_payload_base<Timestamped<canzero_frame> >::_Optional_payload_base<Timestamped<canzero_frame> >(std::in_place_t arg1, Timestamped<canzero_frame>&& arg2)
{
    void* result = arg1;
    std::_Optional_payload_base<Timestamped<canzero_frame> >::_Storage<Timestamped<canzero_frame>, true>::_Storage<Timestamped<canzero_frame> >(
        result, std::forward<Timestamped<canzero_frame> >(arg2));
    *(result + 0x14) = 1;
    return result;
}

void* std::_Optional_base<Timestamped<canzero_frame>, false, false>::_Optional_base<Timestamped<canzero_frame>, false>(std::in_place_t arg1, Timestamped<canzero_frame>&& arg2)
{
    return std::_Optional_payload<Timestamped<canzero_frame>, true, false, false>::_Optional_payload<Timestamped<canzero_frame> >(std::in_place_t, Timestamped<canzero_frame>&&)(
        arg1, std::forward<Timestamped<canzero_frame> >(arg2));
}

int64_t std::_Optional_payload_base<Timestamped<canzero_frame> >::_M_get() __pure
{
    int64_t result;
    return result;
}

Timestamped<canzero_frame>&&& std::move<Timestamped<canzero_frame>&>(Timestamped<canzero_frame>&&& arg1) __pure
{
    return arg1;
}

Timestamped<canzero_frame>&& std::_Optional_payload_base<Timestamped<canzero_frame> >::_M_construct<Timestamped<canzero_frame> >(Timestamped<canzero_frame>&& arg1)
{
    std::remove_reference<Timestamped<canzero_frame> >::type* rsi;
    std::remove_reference<Timestamped<canzero_frame> >::type* rax_1 =
        std::forward<Timestamped<canzero_frame> >(rsi);
    std::_Construct<Timestamped<canzero_frame>, Timestamped<canzero_frame> >(
        std::__addressof<Timestamped<canzero_frame> >(arg1), rax_1);
    *(arg1 + 0x14) = 1;
    return arg1;
}

void* std::_Optional_payload_base<Timestamped<canzero_frame> >::_M_reset()
{
    void* result;
    
    if (*(result + 0x14))
        return std::_Optional_payload_base<Timestamped<canzero_frame> >::_M_destroy();
    
    *(result + 0x14) = 0;
    return result;
}

std::remove_reference<canzero_frame>::type& std::forward<canzero_frame>(std::remove_reference<canzero_frame>::type& arg1) __pure
{
    return arg1;
}

void* std::_Optional_payload<canzero_frame, true, true, true>::_Optional_payload<canzero_frame>(std::in_place_t, canzero_frame&&)(int64_t arg1, canzero_frame* arg2)
{
    return std::_Optional_payload_base<canzero_frame>::_Optional_payload_base<canzero_frame>(arg1, 
        arg2);
}

int64_t std::_Optional_payload_base<canzero_frame>::_Storage<canzero_frame, true>::_Storage<canzero_frame>(std::in_place_t arg1, canzero_frame&& arg2)
{
    int64_t* var_10 = arg1;
    std::remove_reference<canzero_frame>::type* rax_1 = std::forward<canzero_frame>(arg2);
    int64_t rdx = *(rax_1 + 8);
    int64_t result = *rax_1;
    *var_10 = result;
    var_10[1] = rdx;
    return result;
}

void* std::_Optional_payload_base<canzero_frame>::_Optional_payload_base<canzero_frame>(std::in_place_t arg1, canzero_frame&& arg2)
{
    void* result = arg1;
    std::_Optional_payload_base<canzero_frame>::_Storage<canzero_frame, true>::_Storage<canzero_frame>(
        result, std::forward<canzero_frame>(arg2));
    *(result + 0x10) = 1;
    return result;
}

void* std::_Optional_base<canzero_frame, true, true>::_Optional_base<canzero_frame, false>(std::in_place_t arg1, canzero_frame&& arg2)
{
    return std::_Optional_payload<canzero_frame, true, true, true>::_Optional_payload<canzero_frame>(std::in_place_t, canzero_frame&&)(
        arg1, std::forward<canzero_frame>(arg2));
}

int64_t std::_Optional_payload_base<canzero_frame>::_M_get() __pure
{
    int64_t result;
    return result;
}

char* std::string::_S_copy(char* arg1, char const* arg2, uint64_t arg3)
{
    if (arg3 != 1)
        return std::char_traits<char>::copy(arg1, arg2, arg3);
    
    return std::char_traits<char>::assign(arg1, arg2);
}

char* std::addressof<char const>(char const& arg1)
{
    return std::__addressof<char const>(arg1);
}

int64_t std::string::_M_get_allocator() __pure
{
    int64_t result;
    return result;
}

int64_t std::string::max_size()
{
    int64_t var_10 = std::string::_M_get_allocator();
    return 0x7fffffffffffffff;
}

int64_t std::string::_S_allocate(std::allocator<char>& arg1, uint64_t arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t result = std::__new_allocator<char>::allocate(arg1, arg2);
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

uint64_t std::string::_M_replace(uint64_t arg1, uint64_t arg2, char const* arg3, uint64_t arg4)
{
    uint64_t r8;
    std::string::_M_check_length(arg1, arg3, r8);
    int64_t rax_2 = std::string::size();
    int64_t rax_7;
    rax_7 = std::string::capacity() >= r8 + rax_2 - arg3;
    
    if (!rax_7)
        std::string::_M_mutate(arg1, arg2, arg3, arg4);
    else
    {
        char* rax_10 = std::string::_M_data() + arg2;
        void* rax_13 = rax_2 - arg2 - arg3;
        uint64_t rax_38;
        rax_38 = std::string::_M_disjunct(arg1);
        
        if (!rax_38)
            std::string::_M_replace_cold(arg1, rax_10, arg3, arg4, r8);
        else
        {
            if (rax_13 && arg3 != r8)
                std::string::_S_move(&rax_10[r8], rax_10 + arg3, rax_13);
            
            if (r8)
                std::string::_S_copy(rax_10, arg4, r8);
        }
    }
    
    std::string::_M_set_length(arg1);
    return arg1;
}

char const* std::string::_M_check(uint64_t arg1, char const* arg2)
{
    int64_t rax_1;
    rax_1 = std::string::size() < arg2;
    int64_t rdx;
    
    if (rax_1)
        std::__throw_out_of_range_fmt("%s: __pos (which is %zu) > this-…", rdx, arg2, 
            std::string::size());
    return arg2;
}

int32_t std::__countl_zero<uint64_t>(uint64_t arg1)
{
    int32_t var_1c = 0x40;
    
    if (!arg1)
        return 0x40;
    
    int32_t var_18_1 = 0x40;
    int32_t var_14_1 = 0x40;
    int32_t var_10_1 = 0x20;
    int32_t var_c_1 = 0;
    uint64_t rflags_1;
    int32_t rax_1;
    rax_1 = __bsr_gprv_memv(arg1);
    return rax_1 ^ 0x3f;
}

int32_t std::__countl_zero<uint128_t>(uint128_t arg1)
{
    int32_t var_34 = 0x80;
    int64_t rsi;
    int64_t rdi;
    
    if (!(rdi | rsi))
        return 0x80;
    
    int32_t var_30_1 = 0x40;
    int32_t var_2c_1 = 0x40;
    int32_t var_28_1 = 0x20;
    
    if (!rsi)
    {
        int64_t var_18_1 = -1;
        uint64_t rflags_2;
        int32_t rax_8;
        rax_8 = __bsr_gprv_memv(rdi);
        return (rax_8 ^ 0x3f) + 0x40;
    }
    
    int32_t var_24_1 = 0;
    uint64_t rflags_1;
    int32_t rax_6;
    rax_6 = __bsr_gprv_memv(rsi);
    return rax_6 ^ 0x3f;
}

char32_t const*& std::__advance<char32_t const*, int64_t>(char32_t const*& arg1, int64_t arg2, std::random_access_iterator_tag arg3)
{
    *arg1 = &(*arg1)[arg2];
    return arg1;
}

uint32_t const*& std::__advance<uint32_t const*, int64_t>(uint32_t const*& arg1, int64_t arg2, std::random_access_iterator_tag arg3)
{
    *arg1 = &(*arg1)[arg2];
    return arg1;
}

bool __ops::_Iter_equals_val<char32_t const>::operator()<char32_t const*>(char32_t const* arg1)
{
    int32_t rax_3;
    int32_t* rsi;
    rax_3 = *rsi == **arg1;
    return rax_3;
}

uint64_t std::basic_string_view<char>::remove_suffix(uint64_t arg1)
{
    int64_t rsi;
    *arg1 -= rsi;
    return arg1;
}

std::__format::_Sink<char>& std::__addressof<std::__format::_Sink<char> >(std::__format::_Sink<char>& arg1) __pure
{
    return arg1;
}

uint64_t std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char> >::_M_type(uint64_t arg1)
{
    int64_t rsi;
    return *arg1 >> 4 >> ((rsi << 2) + rsi) & 0x1f;
}

char* const& __normal_iterator<char*, std::span<char, 18446744073709551615ul> >::__normal_iterator(char* const& arg1)
{
    char** rsi;
    *arg1 = *rsi;
    return arg1;
}

int64_t __normal_iterator<char*, std::span<char, 18446744073709551615ul> >::operator->()
{
    int64_t* rdi;
    return *rdi;
}

int64_t std::__format::_Sink<char>::_M_bump(uint64_t arg1)
{
    return __normal_iterator<char*, std::span<char, 18446744073709551615ul> >::operator+=(arg1
        + 0x18);
}

int64_t __normal_iterator<char*, std::span<char, 18446744073709551615ul> >::base() __pure
{
    int64_t result;
    return result;
}

uint64_t std::span<char, 18446744073709551615ul>::span<char*>(char* arg1, uint64_t arg2)
{
    *arg1 = std::to_address<char>(arg2);
    return std::__extent_storage<18446744073709551615ul>::__extent_storage(&arg1[8]);
}

int64_t std::string::_M_check_length(uint64_t arg1, uint64_t arg2, char const* arg3)
{
    std::string::max_size();
    int64_t result;
    result = 0x7fffffffffffffff + arg2 - std::string::size() < arg3;
    
    if (!result)
        return result;
    
    char* what;
    std::__throw_length_error(what);
    /* no return */
}

char const* std::string::_M_append(char const* arg1, uint64_t arg2)
{
    uint64_t rdx;
    int64_t rax_2 = std::string::size() + rdx;
    int64_t rax_4;
    rax_4 = std::string::capacity() >= rax_2;
    
    if (!rax_4)
        std::string::_M_mutate(arg1, std::string::size(), nullptr, arg2);
    else if (rdx)
        std::string::_S_copy(std::string::_M_data() + std::string::size(), arg2, rdx);
    
    std::string::_M_set_length(arg1);
    return arg1;
}

std::__format::_Arg_store<std::basic_format_context<std::__format::_Sink_iter<char>, char>, std::basic_format_context<std::__format::_Sink_iter<char>, char> > const& std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char> >::basic_format_args<>(std::__format::_Arg_store<std::basic_format_context<std::__format::_Sink_iter<char>, char>, std::basic_format_context<std::__format::_Sink_iter<char>, char> > const& arg1)
{
    int64_t rsi;
    int64_t var_18 = rsi;
    *arg1 &= 0xf0;
    *arg1 = *arg1 & 0xf;
    *(arg1 + 8) = 0;
    return arg1;
}

int64_t std::reverse_iterator<std::__format::_Arg_t const*>::operator*()
{
    int64_t* rdi;
    return *rdi - 1;
}

std::__format::_Arg_t* std::array<std::__format::_Arg_t, 1ul>::rend()
{
    std::array<std::__format::_Arg_t, 1ul>::begin();
    std::__format::_Arg_t* result;
    std::reverse_iterator<std::__format::_Arg_t const*>::reverse_iterator(result);
    return result;
}

int64_t std::array<std::__format::_Arg_t, 1ul>::begin()
{
    return std::array<std::__format::_Arg_t, 1ul>::data();
}

int64_t std::array<std::__format::_Arg_t, 1ul>::data() __pure
{
    int64_t result;
    return result;
}

std::__format::_Arg_t const* std::reverse_iterator<std::__format::_Arg_t const*>::reverse_iterator(std::__format::_Arg_t const* arg1)
{
    int64_t rsi;
    *arg1 = rsi;
    return arg1;
}

int64_t std::reverse_iterator<std::__format::_Arg_t const*>::base()
{
    int64_t* rdi;
    return *rdi;
}

bool std::operator==<std::__format::_Arg_t const*>(std::reverse_iterator<std::__format::_Arg_t const*> const& arg1, std::reverse_iterator<std::__format::_Arg_t const*> const& arg2)
{
    int64_t rax_3;
    rax_3 = std::reverse_iterator<std::__format::_Arg_t const*>::base()
        == std::reverse_iterator<std::__format::_Arg_t const*>::base();
    return rax_3;
}

auto std::__format::__pack_arg_types<5, 1ul>(std::array<std::__format::_Arg_t, 1ul> const& arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t result = 0;
    std::array<std::__format::_Arg_t, 1ul>::rbegin();
    
    while (true)
    {
        std::array<std::__format::_Arg_t, 1ul>::rend();
        std::reverse_iterator<std::__format::_Arg_t const*> var_30;
        void var_28;
        
        if (std::operator==<std::__format::_Arg_t const*>(&var_30, &var_28) == 1)
            break;
        
        result = *std::reverse_iterator<std::__format::_Arg_t const*>::operator*() | result << 5;
        std::reverse_iterator<std::__format::_Arg_t const*>::operator++();
    }
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

std::__format::_Arg_t* std::array<std::__format::_Arg_t, 1ul>::rbegin()
{
    std::array<std::__format::_Arg_t, 1ul>::end();
    std::__format::_Arg_t* result;
    std::reverse_iterator<std::__format::_Arg_t const*>::reverse_iterator(result);
    return result;
}

int64_t std::array<std::__format::_Arg_t, 1ul>::end()
{
    return std::array<std::__format::_Arg_t, 1ul>::data() + 1;
}

int64_t* std::reverse_iterator<std::__format::_Arg_t const*>::operator++()
{
    int64_t* result;
    *result -= 1;
    return result;
}

int64_t std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char> >::basic_format_args<std::basic_string_view<char> >(std::__format::_Arg_store<std::basic_format_context<std::__format::_Sink_iter<char>, char>, std::basic_string_view<char> > const& arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    *arg1 = (*arg1 & 0xf0) | 1;
    char var_11 = 0xb;
    int64_t rcx_1 = (0xfffffffffffffff & std::__format::__pack_arg_types<5, 1ul>(&var_11)) << 4;
    *arg1 = (*arg1 & 0xf) | rcx_1;
    int64_t rsi;
    *(arg1 + 8) = rsi;
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char> >::basic_format_args<uint32_t>(std::__format::_Arg_store<std::basic_format_context<std::__format::_Sink_iter<char>, char>, uint32_t> const& arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    *arg1 = (*arg1 & 0xf0) | 1;
    char var_11 = 4;
    int64_t rcx_1 = (0xfffffffffffffff & std::__format::__pack_arg_types<5, 1ul>(&var_11)) << 4;
    *arg1 = (*arg1 & 0xf) | rcx_1;
    int64_t rsi;
    *(arg1 + 8) = rsi;
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

bool std::__raise_and_add<uint16_t>(uint16_t& arg1, int32_t arg2, uint8_t arg3)
{
    char rcx = 0;
    int64_t rax_4 = *arg1 * arg2;
    
    if (rax_4 != rax_4)
        rcx = 1;
    
    *arg1 = rax_4;
    char rcx_1;
    
    if (!(rcx & 1))
    {
        uint16_t rdx_4 = *arg1;
        uint16_t rax_9 = arg3;
        rcx_1 = 0;
        
        if (rax_9 + rdx_4 < rax_9)
            rcx_1 = 1;
        
        *arg1 = rax_9 + rdx_4;
    }
    
    char rax_13;
    
    if (!(rcx & 1) && !(rcx_1 & 1))
        rax_13 = 0;
    else
        rax_13 = 1;
    
    if (!rax_13)
        return 1;
    
    return 0;
}

std::remove_reference<uint16_t&>::type& std::forward<uint16_t&>(std::remove_reference<uint16_t&>::type& arg1) __pure
{
    return arg1;
}

std::remove_reference<char const*&>::type& std::forward<char const*&>(std::remove_reference<char const*&>::type& arg1) __pure
{
    return arg1;
}

int64_t std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_read_utf8()::'lambda'()::operator()()
{
    int64_t* rdi;
    char* rax_1 = rdi[1];
    *rax_1 += 1;
    *rdi;
    int64_t* rax_4 = std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_curr();
    *rax_4 += 1;
    return *rax_4;
}

int64_t std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_read_utf8()
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    void* rdi;
    void* var_38 = rdi;
    int64_t var_30 = *std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_curr();
    int32_t var_3c = 0;
    char var_43 = 0x80;
    char var_42 = 0xbf;
    int64_t* rax_6 = std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_curr();
    char* rdx = *rax_6;
    *rax_6 = &rdx[1];
    uint8_t rax_7 = *rdx;
    char var_44 = 1;
    void* var_28 = rdi;
    char* var_20 = &var_44;
    uint8_t var_3c_1;
    
    if (rax_7 >= 0)
        var_3c_1 = rax_7;
    else if (rax_7 > 0xc1)
    {
        int64_t rax_15;
        rax_15 = *std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_curr()
            == *(rdi + 0x20);
        
        if (rax_15)
        {
            std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_S_error();
            var_3c_1 = -3;
        }
        else if (rax_7 <= 0xdf)
        {
            char rax_22 = **std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_curr();
            
            if (rax_22 >= 0 || rax_22 > 0xbf)
            {
                std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_S_error();
                var_3c_1 = -3;
            }
            else
            {
                var_3c_1 = (rax_22 & 0x3f) | (rax_7 & 0x1f) << 6;
                std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_read_utf8()::'lambda'()::operator()();
            }
        }
        else if (rax_7 <= 0xef)
        {
            char rax_30;
            
            if (rax_7 != 0xe0)
                rax_30 = -0x80;
            else
                rax_30 = -0x60;
            
            char rax_31;
            
            if (rax_7 != 0xed)
                rax_31 = -0x41;
            else
                rax_31 = -0x61;
            
            char rax_37 = **std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_curr();
            
            if (rax_37 < rax_30 || rax_31 < rax_37)
            {
                std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_S_error();
                var_3c_1 = -3;
            }
            else
            {
                int64_t rax_43;
                rax_43 = std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_read_utf8()::'lambda'()::operator()()
                    == *(rdi + 0x20);
                
                if (!rax_43)
                {
                    char rax_53 = **std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_curr();
                    
                    if (rax_53 >= 0 || rax_53 > 0xbf)
                    {
                        std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_S_error();
                        var_3c_1 = -3;
                    }
                    else
                    {
                        var_3c_1 = (rax_53 & 0x3f) | ((rax_37 & 0x3f) | (rax_7 & 0xf) << 6) << 6;
                        std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_read_utf8()::'lambda'()::operator()();
                    }
                }
                else
                {
                    std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_S_error();
                    var_3c_1 = -3;
                }
            }
        }
        else if (rax_7 > 0xf4)
        {
            std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_S_error();
            var_3c_1 = -3;
        }
        else
        {
            char rax_61;
            
            if (rax_7 != 0xf0)
                rax_61 = -0x80;
            else
                rax_61 = -0x70;
            
            char rax_62;
            
            if (rax_7 != 0xf4)
                rax_62 = -0x41;
            else
                rax_62 = -0x71;
            
            char rax_68 = **std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_curr();
            
            if (rax_68 < rax_61 || rax_62 < rax_68)
            {
                std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_S_error();
                var_3c_1 = -3;
            }
            else
            {
                int64_t rax_74;
                rax_74 = std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_read_utf8()::'lambda'()::operator()()
                    == *(rdi + 0x20);
                
                if (!rax_74)
                {
                    char rax_84 = **std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_curr();
                    
                    if (rax_84 >= 0 || rax_84 > 0xbf)
                    {
                        std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_S_error();
                        var_3c_1 = -3;
                    }
                    else
                    {
                        int64_t rax_89;
                        rax_89 = std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_read_utf8()::'lambda'()::operator()()
                            == *(rdi + 0x20);
                        
                        if (!rax_89)
                        {
                            char rax_99 = **std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_curr();
                            
                            if (rax_99 >= 0 || rax_99 > 0xbf)
                            {
                                std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_S_error();
                                var_3c_1 = -3;
                            }
                            else
                            {
                                var_3c_1 = (rax_99 & 0x3f)
                                    | ((rax_84 & 0x3f) | ((rax_68 & 0x3f) | (rax_7 & 7) << 6) << 6)
                                    << 6;
                                std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_read_utf8()::'lambda'()::operator()();
                            }
                        }
                        else
                        {
                            std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_S_error();
                            var_3c_1 = -3;
                        }
                    }
                }
                else
                {
                    std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_S_error();
                    var_3c_1 = -3;
                }
            }
        }
    }
    else
    {
        std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_S_error();
        var_3c_1 = -3;
    }
    
    std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_update(
        rdi, var_3c_1);
    std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_Guard<char const*>::~_Guard();
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

void* std::_Construct<Timestamped<canzero_frame>, Timestamped<canzero_frame> >(Timestamped<canzero_frame>* arg1, Timestamped<canzero_frame>&& arg2)
{
    return Timestamped<canzero_frame>::Timestamped(operator new(0x14, arg1));
}

void* std::_Optional_payload_base<Timestamped<canzero_frame> >::_M_destroy()
{
    void* result;
    *(result + 0x14) = 0;
    return result;
}

char const& std::__addressof<char const>(char const& arg1) __pure
{
    return arg1;
}

int64_t std::string::_M_disjunct(char const* arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    std::string::_M_data();
    char var_22;
    char* rsi;
    char rax_3 = std::less<char const*>::operator()(&var_22, rsi);
    char rax_8;
    char var_21;
    
    if (!rax_3)
        rax_8 = std::less<char const*>::operator()(&var_21, 
            std::string::_M_data() + std::string::size());
    int64_t result;
    
    if (!rax_3 && !rax_8)
        result = 0;
    else
        result = 1;
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

char* std::string::_S_move(char* arg1, char const* arg2, uint64_t arg3)
{
    if (arg3 != 1)
        return std::char_traits<char>::move(arg1, arg2, arg3);
    
    return std::char_traits<char>::assign(arg1, arg2);
}

int64_t std::string::_M_mutate(uint64_t arg1, uint64_t arg2, char const* arg3, uint64_t arg4)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    void* rax_4 = std::string::length() - arg2 - arg3;
    uint64_t r8;
    void* var_28 = std::string::length() + r8 - arg3;
    std::string::capacity();
    char* rax_12 = std::string::_M_create(arg1, &var_28);
    
    if (arg2)
        std::string::_S_copy(rax_12, std::string::_M_data(), arg2);
    
    if (arg4 && r8)
        std::string::_S_copy(&rax_12[arg2], arg4, r8);
    
    if (rax_4)
        std::string::_S_copy(&rax_12[arg2 + r8], std::string::_M_data() + &arg3[arg2], rax_4);
    
    std::string::_M_dispose();
    std::string::_M_data(arg1);
    std::string::_M_capacity(arg1);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::span<char, 18446744073709551615ul>::subspan(uint64_t arg1, uint64_t arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t rdx;
    
    if (rdx == -1)
        int64_t var_50 = std::span<char, 18446744073709551615ul>::size() - arg2;
    
    int64_t result;
    std::span<char, 18446744073709551615ul>::span<char*>(&result, 
        std::span<char, 18446744073709551615ul>::data() + arg2);
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

int64_t __normal_iterator<char*, std::span<char, 18446744073709551615ul> >::operator+=(int64_t arg1)
{
    int64_t rsi;
    *arg1 += rsi;
    return arg1;
}

char* std::to_address<char>(char* arg1)
{
    return std::__to_address<char>(arg1);
}

std::__format::_Arg_value<std::basic_format_context<std::__format::_Sink_iter<char>, char> > std::__format::_Arg_store<std::basic_format_context<std::__format::_Sink_iter<char>, char>, std::basic_string_view<char> >::_S_make_elt<std::string>(std::string& arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t result;
    std::basic_format_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char> >::basic_format_arg<std::string>(
        &result);
    int64_t var_30;
    int64_t var_40 = var_30;
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

std::__format::_Arg_value<std::basic_format_context<std::__format::_Sink_iter<char>, char> > std::__format::_Arg_store<std::basic_format_context<std::__format::_Sink_iter<char>, char>, uint32_t>::_S_make_elt<uint16_t>(uint16_t& arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t result;
    std::basic_format_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char> >::basic_format_arg<uint16_t>(
        &result);
    int64_t var_30;
    int64_t var_40 = var_30;
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

char*** std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_Guard<char const*>::~_Guard()
{
    int64_t* rdi;
    char** rbx = *std::move<char const*&>(&rdi[1]);
    *rdi;
    char*** result = std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_curr();
    *result = rbx;
    return result;
}

int64_t std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_S_error()
{
    int64_t rdi;
    int64_t var_20 = rdi;
    int32_t var_14 = 0xfffd;
    void* fsbase;
    *(fsbase + 0x28);
    
    if (*(fsbase + 0x28) == *(fsbase + 0x28))
        return 0xfffd;
    
    __stack_chk_fail();
    /* no return */
}

uint64_t std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_update(char32_t arg1, uint8_t arg2)
{
    uint64_t result = arg1;
    char rdx;
    *(result + 0x1a) = rdx;
    *(result + 0x18) = 0;
    *std::array<char32_t, 1ul>::operator[](result) = arg2;
    *(result + 0x19) = 1;
    return result;
}

char const*& std::__advance<char const*, int64_t>(char const*& arg1, int64_t arg2, std::random_access_iterator_tag arg3)
{
    *arg1 = &(*arg1)[arg2];
    return arg1;
}

void* decltype(::new((void*)(0)) Timestamped<canzero_frame>(declval<Timestamped<canzero_frame>>())) std::construct_at<Timestamped<canzero_frame>, Timestamped<canzero_frame>>(Timestamped<canzero_frame>*, Timestamped<canzero_frame>&&)(void* arg1, std::remove_reference<Timestamped<canzero_frame> >::type* arg2)
{
    void* result = operator new(0x14, arg1);
    Timestamped<canzero_frame>::Timestamped(result);
    return result;
}

int64_t std::__new_allocator<char>::deallocate(char* arg1, uint64_t arg2)
{
    char* var_10 = arg1;
    uint64_t rdx;
    return operator delete(arg2, rdx);
}

char const* std::less<char const*>::operator()(char const* arg1, char const* arg2) __pure
{
    char const* var_10 = arg1;
    char const* result;
    char* rdx;
    result = arg2 < rdx;
    return result;
}

int64_t std::basic_format_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char> >::basic_format_arg<std::string>(std::string& arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    std::string& var_40 = arg1;
    uint64_t rax_3 = std::string::data();
    std::string::size();
    int64_t var_38;
    std::basic_string_view<char>::basic_string_view(&var_38, rax_3);
    std::basic_format_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char> >::_M_set<std::basic_string_view<char> >(
        arg1);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int32_t* std::basic_format_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char> >::basic_format_arg<uint16_t>(uint16_t& arg1)
{
    uint16_t& var_10 = arg1;
    int16_t* rsi;
    *rsi;
    return std::basic_format_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char> >::_M_set<uint32_t>(
        arg1);
}

int64_t std::array<char32_t, 1ul>::operator[](uint64_t arg1) __pure
{
    int64_t rsi;
    return arg1 + (rsi << 2);
}

int64_t std::__new_allocator<char>::allocate(uint64_t arg1, void const* arg2)
{
    int64_t rdx;
    int64_t var_30 = rdx;
    uint64_t var_10 = arg1;
    uint64_t rax_2;
    rax_2 = 0x7fffffffffffffff < arg2;
    
    if (!rax_2)
        return operator new(arg2);
    
    std::__throw_bad_alloc();
    /* no return */
}

int64_t std::basic_format_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char> >::_M_set<std::basic_string_view<char> >(std::basic_string_view<char> arg1)
{
    int64_t* var_40 = arg1;
    void* fsbase;
    int64_t rax_2 = *(fsbase + 0x28);
    var_40[2] = 0xb;
    int64_t rsi;
    *var_40 = rsi;
    int64_t rdx;
    var_40[1] = rdx;
    
    if (rax_2 == *(fsbase + 0x28))
        return rax_2 - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int32_t* std::basic_format_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char> >::_M_set<uint32_t>(uint32_t arg1)
{
    int32_t* result = arg1;
    result[4] = 4;
    int32_t rsi;
    *result = rsi;
    return result;
}

int64_t canzero::telemetry::CanzeroConnection::~CanzeroConnection()
{
    return telemetry_board::TcpConnection::~TcpConnection();
}

int64_t std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_M_on_chars(char const* arg1)
{
    int64_t rsi;
    int64_t var_48 = rsi;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_38;
    std::basic_string_view<char>::basic_string_view<char const*, char const*>(&var_38, 
        std::__format::_Scanner<char>::begin());
    int64_t rbx = *(arg1 + 0x30);
    *(arg1 + 0x30);
    std::__format::__write<std::__format::_Sink_iter<char>, char>(
        std::basic_format_context<std::__format::_Sink_iter<char>, char>::out(), var_38);
    std::basic_format_context<std::__format::_Sink_iter<char>, char>::advance_to(rbx);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

void auto std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_M_format_arg(unsigned long)::'lambda'(auto&)::operator()<std::monostate>(auto&) const() __noreturn
{
    int64_t rdi;
    int64_t var_10 = rdi;
    int64_t rsi;
    int64_t var_18 = rsi;
    std::__format::__invalid_arg_id_in_format_string();
    /* no return */
}

int64_t auto std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_M_format_arg(unsigned long)::'lambda'(auto&)::operator()<bool>(auto&) const(int64_t* arg1, char* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_2c = 0;
    int32_t var_24 = 0;
    int32_t var_24_1 = 0x20;
    char* rbx = *arg1 + 8;
    *arg1;
    std::__format::__formatter_int<char>::_M_parse<bool>(&var_2c);
    std::basic_format_parse_context<char>::advance_to(rbx);
    int64_t rbx_1 = *(*arg1 + 0x30);
    *(*arg1 + 0x30);
    std::formatter<bool, char>::format<std::__format::_Sink_iter<char> >(&var_2c, *arg2);
    std::basic_format_context<std::__format::_Sink_iter<char>, char>::advance_to(rbx_1);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t auto std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_M_format_arg(unsigned long)::'lambda'(auto&)::operator()<char>(auto&) const(int64_t* arg1, char* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_2c = 0;
    int32_t var_24 = 0;
    int32_t var_24_1 = 0x20;
    char* rbx = *arg1 + 8;
    *arg1;
    std::formatter<char, char>::parse(&var_2c);
    std::basic_format_parse_context<char>::advance_to(rbx);
    int64_t rbx_1 = *(*arg1 + 0x30);
    *(*arg1 + 0x30);
    std::formatter<char, char>::format<std::__format::_Sink_iter<char> >(&var_2c, *arg2);
    std::basic_format_context<std::__format::_Sink_iter<char>, char>::advance_to(rbx_1);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t auto std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_M_format_arg(unsigned long)::'lambda'(auto&)::operator()<int>(auto&) const(int64_t* arg1, int32_t* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_2c = 0;
    int32_t var_24 = 0;
    int32_t var_24_1 = 0x20;
    char* rbx = *arg1 + 8;
    *arg1;
    std::__format::__formatter_int<char>::_M_parse<int32_t>(&var_2c);
    std::basic_format_parse_context<char>::advance_to(rbx);
    int64_t rbx_1 = *(*arg1 + 0x30);
    *(*arg1 + 0x30);
    std::formatter<int32_t, char>::format<std::__format::_Sink_iter<char> >(&var_2c, *arg2);
    std::basic_format_context<std::__format::_Sink_iter<char>, char>::advance_to(rbx_1);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t auto std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_M_format_arg(unsigned long)::'lambda'(auto&)::operator()<unsigned int>(auto&) const(int64_t* arg1, int32_t* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_2c = 0;
    int32_t var_24 = 0;
    int32_t var_24_1 = 0x20;
    char* rbx = *arg1 + 8;
    *arg1;
    std::__format::__formatter_int<char>::_M_parse<uint32_t>(&var_2c);
    std::basic_format_parse_context<char>::advance_to(rbx);
    int64_t rbx_1 = *(*arg1 + 0x30);
    *(*arg1 + 0x30);
    std::formatter<uint32_t, char>::format<std::__format::_Sink_iter<char> >(&var_2c, *arg2);
    std::basic_format_context<std::__format::_Sink_iter<char>, char>::advance_to(rbx_1);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t auto std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_M_format_arg(unsigned long)::'lambda'(auto&)::operator()<long long>(auto&) const(int64_t* arg1, int64_t* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_2c = 0;
    int32_t var_24 = 0;
    int32_t var_24_1 = 0x20;
    char* rbx = *arg1 + 8;
    *arg1;
    std::__format::__formatter_int<char>::_M_parse<int64_t>(&var_2c);
    std::basic_format_parse_context<char>::advance_to(rbx);
    int64_t rbx_1 = *(*arg1 + 0x30);
    *(*arg1 + 0x30);
    std::formatter<int64_t, char>::format<std::__format::_Sink_iter<char> >(&var_2c, *arg2);
    std::basic_format_context<std::__format::_Sink_iter<char>, char>::advance_to(rbx_1);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t auto std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_M_format_arg(unsigned long)::'lambda'(auto&)::operator()<unsigned long long>(auto&) const(int64_t* arg1, int64_t* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_2c = 0;
    int32_t var_24 = 0;
    int32_t var_24_1 = 0x20;
    char* rbx = *arg1 + 8;
    *arg1;
    std::__format::__formatter_int<char>::_M_parse<uint64_t>(&var_2c);
    std::basic_format_parse_context<char>::advance_to(rbx);
    int64_t rbx_1 = *(*arg1 + 0x30);
    *(*arg1 + 0x30);
    std::formatter<uint64_t, char>::format<std::__format::_Sink_iter<char> >(&var_2c, *arg2);
    std::basic_format_context<std::__format::_Sink_iter<char>, char>::advance_to(rbx_1);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t auto std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_M_format_arg(unsigned long)::'lambda'(auto&)::operator()<float>(auto&) const(int64_t* arg1, int32_t* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_2c = 0;
    int32_t var_24 = 0;
    int32_t var_24_1 = 0x20;
    char* rbx = *arg1 + 8;
    *arg1;
    std::__format::__formatter_fp<char>::parse(&var_2c);
    std::basic_format_parse_context<char>::advance_to(rbx);
    int64_t rbx_1 = *(*arg1 + 0x30);
    *(*arg1 + 0x30);
    std::formatter<float, char>::format<std::__format::_Sink_iter<char> >(*arg2, &var_2c);
    std::basic_format_context<std::__format::_Sink_iter<char>, char>::advance_to(rbx_1);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t auto std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_M_format_arg(unsigned long)::'lambda'(auto&)::operator()<double>(auto&) const(int64_t* arg1, int64_t* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_2c = 0;
    int32_t var_24 = 0;
    int32_t var_24_1 = 0x20;
    char* rbx = *arg1 + 8;
    *arg1;
    std::__format::__formatter_fp<char>::parse(&var_2c);
    std::basic_format_parse_context<char>::advance_to(rbx);
    int64_t rbx_1 = *(*arg1 + 0x30);
    *(*arg1 + 0x30);
    std::formatter<double, char>::format<std::__format::_Sink_iter<char> >(*arg2, &var_2c);
    std::basic_format_context<std::__format::_Sink_iter<char>, char>::advance_to(rbx_1);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t auto std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_M_format_arg(unsigned long)::'lambda'(auto&)::operator()<long double>(auto&) const(int64_t* arg1, int80_t* arg2, long double arg3)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_2c = 0;
    int32_t var_24 = 0;
    int32_t var_24_1 = 0x20;
    char* rbx = *arg1 + 8;
    *arg1;
    std::__format::__formatter_fp<char>::parse(&var_2c);
    std::basic_format_parse_context<char>::advance_to(rbx);
    int64_t rbx_1 = *(*arg1 + 0x30);
    *(*arg1 + 0x30);
    int80_t var_58 = *arg2;
    std::formatter<long double, char>::format<std::__format::_Sink_iter<char> >(arg3, &var_2c);
    std::basic_format_context<std::__format::_Sink_iter<char>, char>::advance_to(rbx_1);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t auto std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_M_format_arg(unsigned long)::'lambda'(auto&)::operator()<char const*>(auto&) const(int64_t* arg1, int64_t* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_2c = 0;
    int32_t var_24 = 0;
    int32_t var_24_1 = 0x20;
    char* rbx = *arg1 + 8;
    *arg1;
    std::__format::__formatter_str<char>::parse(&var_2c);
    std::basic_format_parse_context<char>::advance_to(rbx);
    int64_t rbx_1 = *(*arg1 + 0x30);
    *(*arg1 + 0x30);
    std::formatter<char const*, char>::format<std::__format::_Sink_iter<char> >(&var_2c, *arg2);
    std::basic_format_context<std::__format::_Sink_iter<char>, char>::advance_to(rbx_1);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t auto std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_M_format_arg(unsigned long)::'lambda'(auto&)::operator()<std::basic_string_view<char, std::char_traits<char>>>(auto&) const(int64_t* arg1, int64_t* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_2c = 0;
    int32_t var_24 = 0;
    int32_t var_24_1 = 0x20;
    char* rbx = *arg1 + 8;
    *arg1;
    std::__format::__formatter_str<char>::parse(&var_2c);
    std::basic_format_parse_context<char>::advance_to(rbx);
    int64_t rbx_1 = *(*arg1 + 0x30);
    *(*arg1 + 0x30);
    arg2[1];
    std::formatter<std::basic_string_view<char>, char>::format<std::__format::_Sink_iter<char> >(
        &var_2c, *arg2);
    std::basic_format_context<std::__format::_Sink_iter<char>, char>::advance_to(rbx_1);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t auto std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_M_format_arg(unsigned long)::'lambda'(auto&)::operator()<void const*>(auto&) const(int64_t* arg1, int64_t* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_2c = 0;
    int32_t var_24 = 0;
    int32_t var_24_1 = 0x20;
    char* rbx = *arg1 + 8;
    *arg1;
    std::formatter<void const*, char>::parse(&var_2c);
    std::basic_format_parse_context<char>::advance_to(rbx);
    int64_t rbx_1 = *(*arg1 + 0x30);
    *(*arg1 + 0x30);
    std::formatter<void const*, char>::format<std::__format::_Sink_iter<char> >(&var_2c, *arg2);
    std::basic_format_context<std::__format::_Sink_iter<char>, char>::advance_to(rbx_1);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t auto std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_M_format_arg(unsigned long)::'lambda'(auto&)::operator()<std::basic_format_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char>>::handle>(auto&) const(int64_t* arg1, int64_t* arg2)
{
    return arg2[1](*arg1 + 8, *(*arg1 + 0x30), *arg2);
}

int64_t auto std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_M_format_arg(unsigned long)::'lambda'(auto&)::operator()<__int128>(auto&) const(int64_t* arg1, int64_t* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_2c = 0;
    int32_t var_24 = 0;
    int32_t var_24_1 = 0x20;
    char* rbx = *arg1 + 8;
    *arg1;
    std::__format::__formatter_int<char>::_M_parse<int128_t>(&var_2c);
    std::basic_format_parse_context<char>::advance_to(rbx);
    int64_t rbx_1 = *(*arg1 + 0x30);
    *(*arg1 + 0x30);
    arg2[1];
    *arg2;
    std::formatter<int128_t, char>::format<std::__format::_Sink_iter<char> >(arg2, &var_2c);
    std::basic_format_context<std::__format::_Sink_iter<char>, char>::advance_to(rbx_1);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t auto std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_M_format_arg(unsigned long)::'lambda'(auto&)::operator()<unsigned __int128>(auto&) const(int64_t* arg1, int64_t* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_2c = 0;
    int32_t var_24 = 0;
    int32_t var_24_1 = 0x20;
    char* rbx = *arg1 + 8;
    *arg1;
    std::__format::__formatter_int<char>::_M_parse<uint128_t>(&var_2c);
    std::basic_format_parse_context<char>::advance_to(rbx);
    int64_t rbx_1 = *(*arg1 + 0x30);
    *(*arg1 + 0x30);
    arg2[1];
    *arg2;
    std::formatter<uint128_t, char>::format<std::__format::_Sink_iter<char> >(arg2, &var_2c);
    std::basic_format_context<std::__format::_Sink_iter<char>, char>::advance_to(rbx_1);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t auto std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_M_format_arg(unsigned long)::'lambda'(auto&)::operator()<_Float128>(auto&) const(int64_t* arg1, int128_t* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_2c = 0;
    int32_t var_24 = 0;
    int32_t var_24_1 = 0x20;
    char* rbx = *arg1 + 8;
    *arg1;
    std::__format::__formatter_fp<char>::parse(&var_2c);
    std::basic_format_parse_context<char>::advance_to(rbx);
    int64_t rbx_1 = *(*arg1 + 0x30);
    std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::formatter<_Float128, char>::format<std::__format::_Sink_iter<char>>(_Float128, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const(
        &var_2c, *(*arg1 + 0x30), *arg2);
    std::basic_format_context<std::__format::_Sink_iter<char>, char>::advance_to(rbx_1);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t decltype(auto) std::basic_format_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char>>::_M_visit<std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_M_format_arg(unsigned long)::'lambda'(auto&)>(auto&&, std::__format::_Arg_t)(int64_t arg1, int64_t arg2, char arg3)
{
    int64_t var_20 = arg1;
    int64_t var_28 = arg2;
    /* jump -> *((arg3 << 2) + &data_412490) + &data_412490 */
}

int64_t decltype(auto) std::visit_format_arg<std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_M_format_arg(unsigned long)::'lambda'(auto&), std::basic_format_context<std::__format::_Sink_iter<char>, char>>(auto&&, std::basic_format_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char>>)(int64_t arg1, char arg2)
{
    void arg_8;
    return decltype(auto) std::basic_format_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char>>::_M_visit<std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_M_format_arg(unsigned long)::'lambda'(auto&)>(auto&&, std::__format::_Arg_t)(
        &arg_8, 
        auto&& std::forward<std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_M_format_arg(unsigned long)::'lambda'(auto&)>(std::remove_reference<auto>::type&)(
            arg1), 
        arg2);
}

int64_t std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_M_format_arg(uint64_t arg1)
{
    int64_t rsi;
    int64_t var_58 = rsi;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    *(arg1 + 0x30);
    int64_t var_38;
    char rsi_2 = std::basic_format_context<std::__format::_Sink_iter<char>, char>::arg(&var_38);
    uint64_t var_40 = arg1;
    int64_t var_78 = var_38;
    int64_t var_30;
    int64_t var_70 = var_30;
    int64_t var_28;
    int64_t var_68 = var_28;
    int64_t var_20;
    int64_t var_60 = var_20;
    decltype(auto) std::visit_format_arg<std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_M_format_arg(unsigned long)::'lambda'(auto&), std::basic_format_context<std::__format::_Sink_iter<char>, char>>(auto&&, std::basic_format_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char>>)(
        &var_40, rsi_2);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

auto std::ranges::__access::_Begin::operator()<std::span<char, 18446744073709551615ul>&>(std::span<char, 18446744073709551615ul>&&& arg1)
{
    std::span<char, 18446744073709551615ul>&&& var_10 = arg1;
    return std::span<char, 18446744073709551615ul>::begin();
}

auto std::ranges::__access::_End::operator()<std::span<char, 18446744073709551615ul>&>(std::span<char, 18446744073709551615ul>&&& arg1)
{
    std::span<char, 18446744073709551615ul>&&& var_10 = arg1;
    return std::span<char, 18446744073709551615ul>::end();
}

int64_t std::__format::_Iter_sink<char, std::__format::_Sink_iter<char> >::_M_overflow()
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    char* var_38 = std::span<char, 18446744073709551615ul>::begin();
    void* rdi;
    operator-<char*, std::span<char, 18446744073709551615ul> >(rdi + 0x18, &var_38);
    int64_t rax_8;
    int64_t rdx_1;
    rax_8 = std::span<char, 18446744073709551615ul>::first(rdi + 8);
    int64_t var_58 = rax_8;
    
    if (*(rdi + 0x128) < 0)
    {
        *std::move<std::__format::_Sink_iter<char>&>(rdi + 0x120);
        char* rax_14;
        int64_t rdx_3;
        rax_14 = std::ranges::__copy_fn::operator()<std::span<char, 18446744073709551615ul>&, std::__format::_Sink_iter<char> >(
            &std::ranges::copy, &var_58);
        var_38 = rax_14;
        *(rdi + 0x120) = rdx_3;
    }
    else if (*(rdi + 0x130) < *(rdi + 0x128))
    {
        int64_t rdx_7 = *(rdi + 0x128) - *(rdi + 0x130);
        int64_t var_48 = 0;
        int64_t var_40_1 = 0;
        int64_t rax_23;
        rax_23 = rdx_7 < std::span<char, 18446744073709551615ul>::size();
        
        if (!rax_23)
        {
            var_48 = var_58;
            int64_t var_40_3 = rdx_1;
        }
        else
        {
            int64_t rax_24;
            int64_t rdx_9;
            rax_24 = std::span<char, 18446744073709551615ul>::first(&var_58);
            var_48 = rax_24;
            int64_t var_40_2 = rdx_9;
        }
        
        *std::move<std::__format::_Sink_iter<char>&>(rdi + 0x120);
        char* rax_29;
        int64_t rdx_12;
        rax_29 = std::ranges::__copy_fn::operator()<std::span<char, 18446744073709551615ul>&, std::__format::_Sink_iter<char> >(
            &std::ranges::copy, &var_48);
        var_38 = rax_29;
        *(rdi + 0x120) = rdx_12;
    }
    
    *(rdi + 0x18) = std::span<char, 18446744073709551615ul>::begin();
    int64_t rax_35 = std::span<char, 18446744073709551615ul>::size();
    *(rdi + 0x130) += rax_35;
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

uint64_t std::__format::_Seq_sink<std::string>::_M_reserve(uint64_t arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    char* var_38 = std::span<char, 18446744073709551615ul>::begin();
    operator-<char*, std::span<char, 18446744073709551615ul> >(arg1 + 0x18, &var_38);
    char* rax_8;
    int64_t rdx_1;
    rax_8 = std::span<char, 18446744073709551615ul>::first(arg1 + 8);
    var_38 = rax_8;
    int64_t rax_9;
    rax_9 = std::span<char, 18446744073709551615ul>::size();
    
    if (rax_9)
        std::__format::_Seq_sink<std::string>::_M_overflow();
    
    int64_t rsi;
    void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::resize_and_overwrite<std::__format::_Seq_sink<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>::_M_reserve(unsigned long)::'lambda'(auto, auto)&>(unsigned long, auto)(
        arg1 + 0x120, rsi + std::string::size(), &var_38);
    std::span<char, 18446744073709551615ul>::span<std::string&>(&var_38);
    std::__format::_Sink<char>::_M_reset(arg1, var_38);
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return arg1;
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::__format::_Seq_sink<std::string>::_M_bump(uint64_t arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    char* var_38 = std::span<char, 18446744073709551615ul>::begin();
    operator-<char*, std::span<char, 18446744073709551615ul> >(arg1 + 0x18, &var_38);
    int64_t rax_8;
    int64_t rdx_1;
    rax_8 = std::span<char, 18446744073709551615ul>::first(arg1 + 8);
    int64_t var_48 = rax_8;
    int64_t var_40 = rdx_1;
    std::span<char, 18446744073709551615ul>::size();
    std::string::resize(arg1 + 0x120);
    std::span<char, 18446744073709551615ul>::span<256ul>(&var_38);
    std::__format::_Sink<char>::_M_reset(arg1, var_38);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

std::__format::_Sink_iter<char> std::__format::__write<std::__format::_Sink_iter<char>, char>(std::__format::_Sink_iter<char> arg1, std::basic_string_view<char> arg2)
{
    int64_t result = arg1;
    void* fsbase;
    int64_t rax_2 = *(fsbase + 0x28);
    int64_t rax_3;
    rax_3 = std::basic_string_view<char>::size();
    
    if (rax_3)
    {
        int64_t rdx;
        int64_t var_20_1 = rdx;
        std::__format::_Sink<char>::_M_write(result);
    }
    
    *(fsbase + 0x28);
    
    if (rax_2 == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

std::__format::_Sink_iter<char>* std::basic_format_context<std::__format::_Sink_iter<char>, char>::advance_to(std::__format::_Sink_iter<char> arg1)
{
    void* var_10 = arg1;
    int64_t rsi;
    int64_t var_18 = rsi;
    std::__format::_Sink_iter<char>* result = *std::move<std::__format::_Sink_iter<char>&>(&var_18);
    *(var_10 + 0x10) = result;
    return result;
}

uint64_t std::basic_format_context<std::__format::_Sink_iter<char>, char>::arg(uint64_t arg1)
{
    uint64_t var_10 = arg1;
    *(arg1 + 0x10) = 0;
    int64_t rdx;
    char* rsi;
    
    if (rdx < (*rsi & 0xf))
    {
        *(arg1 + 0x10) = std::basic_format_args<std::basic_format_context<std::__format::_Sink_iter<char>, char> >::_M_type(
            rsi);
        int64_t* rax_12 = *(rsi + 8) + (rdx << 4);
        int64_t rdx_5 = rax_12[1];
        *arg1 = *rax_12;
        *(arg1 + 8) = rdx_5;
    }
    else if (!(*rsi & 0xf) && rdx < *rsi >> 4)
    {
        int64_t* rsi_2 = *(rsi + 8) + (rdx << 5);
        int64_t rdx_8 = rsi_2[1];
        *arg1 = *rsi_2;
        *(arg1 + 8) = rdx_8;
        *(arg1 + 0x10) = rsi_2[2];
    }
    
    return arg1;
}

int64_t auto&& std::forward<std::__format::_Formatting_scanner<std::__format::_Sink_iter<char>, char>::_M_format_arg(unsigned long)::'lambda'(auto&)>(std::remove_reference<auto>::type&)(int64_t arg1) __pure
{
    return arg1;
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::formatter<bool, char>::format<std::__format::_Sink_iter<char> >(bool arg1, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg2)
{
    return std::__format::__formatter_int<char>::format<std::__format::_Sink_iter<char> >(arg1, 
        arg2);
}

char* std::formatter<char, char>::parse(std::basic_format_parse_context<char>& arg1)
{
    return std::__format::__formatter_int<char>::_M_parse<char>(arg1);
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::formatter<char, char>::format<std::__format::_Sink_iter<char> >(char arg1, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg2)
{
    void* var_10 = arg1;
    char rax = arg2;
    
    if (!(*(var_10 + 1) & 0x78) || (*(var_10 + 1) & 0x78) == 0x38)
        return std::__format::__formatter_int<char>::_M_format_character<std::__format::_Sink_iter<char> >(
            var_10, rax);
    
    if ((*(var_10 + 1) & 0x78) != 0x78)
        return 
            std::__format::__formatter_int<char>::format<uint8_t, std::__format::_Sink_iter<char> >(
            var_10, rax);
    
    return std::basic_format_context<std::__format::_Sink_iter<char>, char>::out();
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::formatter<int32_t, char>::format<std::__format::_Sink_iter<char> >(int32_t arg1, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg2)
{
    return std::__format::__formatter_int<char>::format<int32_t, std::__format::_Sink_iter<char> >(
        arg1, arg2);
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::formatter<uint32_t, char>::format<std::__format::_Sink_iter<char> >(uint32_t arg1, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg2)
{
    return std::__format::__formatter_int<char>::format<uint32_t, std::__format::_Sink_iter<char> >(
        arg1, arg2);
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::formatter<int64_t, char>::format<std::__format::_Sink_iter<char> >(int64_t arg1, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg2)
{
    return std::__format::__formatter_int<char>::format<int64_t, std::__format::_Sink_iter<char> >(
        arg1, arg2);
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::formatter<uint64_t, char>::format<std::__format::_Sink_iter<char> >(uint64_t arg1, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg2)
{
    return std::__format::__formatter_int<char>::format<uint64_t, std::__format::_Sink_iter<char> >(
        arg1, arg2);
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::formatter<float, char>::format<std::__format::_Sink_iter<char> >(float arg1, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg2)
{
    return std::__format::__formatter_fp<char>::format<float, std::__format::_Sink_iter<char> >(
        arg1, arg2);
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::formatter<double, char>::format<std::__format::_Sink_iter<char> >(double arg1, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg2)
{
    return std::__format::__formatter_fp<char>::format<double, std::__format::_Sink_iter<char> >(
        arg1, arg2);
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::formatter<long double, char>::format<std::__format::_Sink_iter<char> >(long double arg1, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg2)
{
    int64_t arg_10;
    int64_t var_20 = arg_10;
    int64_t arg_8;
    int64_t var_28 = arg_8;
    return 
        std::__format::__formatter_fp<char>::format<long double, std::__format::_Sink_iter<char> >(
        arg1, arg2);
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::formatter<char const*, char>::format<std::__format::_Sink_iter<char> >(char const* arg1, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg2)
{
    int64_t rdx;
    int64_t var_50 = rdx;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    std::basic_format_context<std::__format::_Sink_iter<char>, char>* var_38;
    std::basic_string_view<char>::basic_string_view(&var_38);
    int64_t result = std::__format::__formatter_str<char>::format<std::__format::_Sink_iter<char> >(
        arg1, var_38);
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::formatter<std::basic_string_view<char>, char>::format<std::__format::_Sink_iter<char> >(std::basic_string_view<char> arg1, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg2)
{
    int64_t rdx;
    int64_t var_20 = rdx;
    return std::__format::__formatter_str<char>::format<std::__format::_Sink_iter<char> >(arg1, 
        arg2);
}

int64_t* std::formatter<void const*, char>::parse(std::basic_format_parse_context<char>&)::'lambda'()::operator()()
{
    int64_t* rdi;
    int64_t* rdx = rdi[1];
    int64_t* result = *rdi;
    *result = *rdx;
    result[1] = rdx[1];
    return result;
}

int64_t std::formatter<void const*, char>::parse(std::basic_format_parse_context<char>&)::'lambda0'()::operator()()
{
    int64_t* rdi;
    
    if (**rdi != *rdi[1] && ***rdi != 0x7d)
        return 0;
    
    rdi[2];
    std::formatter<void const*, char>::parse(std::basic_format_parse_context<char>&)::'lambda'()::operator()();
    return 1;
}

char* std::formatter<void const*, char>::parse(std::basic_format_parse_context<char>& arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_44 = 0;
    int32_t var_3c = 0;
    int32_t var_3c_1 = 0x20;
    std::basic_format_parse_context<char>* var_58 = std::basic_format_parse_context<char>::end();
    char* result_1 = std::basic_format_parse_context<char>::begin();
    std::basic_format_parse_context<char>& var_38 = arg1;
    int64_t* var_30 = &var_44;
    int64_t* var_28 = &result_1;
    int64_t* var_20 = &var_58;
    std::basic_format_parse_context<char>&* var_18 = &var_38;
    char* result;
    
    if (!std::formatter<void const*, char>::parse(std::basic_format_parse_context<char>&)::'lambda0'()::operator()())
    {
        result_1 = std::__format::_Spec<char>::_M_parse_fill_and_align(&var_44, result_1);
        
        if (!std::formatter<void const*, char>::parse(std::basic_format_parse_context<char>&)::'lambda0'()::operator()())
        {
            result_1 = std::__format::_Spec<char>::_M_parse_zero_fill(&var_44, result_1);
            
            if (!std::formatter<void const*, char>::parse(std::basic_format_parse_context<char>&)::'lambda0'()::operator()())
            {
                result_1 = std::__format::_Spec<char>::_M_parse_width(&var_44, result_1, var_58);
                
                if (result_1 != var_58)
                {
                    if (*result_1 == 0x70)
                        result_1 = &result_1[1];
                    else if (*result_1 == 0x50)
                    {
                        *var_44[1] = (*var_44[1] & 0x87) | 8;
                        result_1 = &result_1[1];
                    }
                }
                
                if (!std::formatter<void const*, char>::parse(std::basic_format_parse_context<char>&)::'lambda0'()::operator()())
                {
                    *(fsbase + 0x28);
                    
                    if (rax != *(fsbase + 0x28))
                    {
                        __stack_chk_fail();
                        /* no return */
                    }
                    
                    std::__format::__failed_to_parse_format_spec();
                    /* no return */
                }
                
                result = result_1;
            }
            else
                result = result_1;
        }
        else
            result = result_1;
    }
    else
        result = result_1;
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

int64_t* std::basic_string_view<char>::basic_string_view()
{
    int64_t* result;
    *result = 0;
    result[1] = 0;
    return result;
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::formatter<void const*, char>::format<std::__format::_Sink_iter<char> >(void const* arg1, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    char var_36[0x10];
    char var_26;
    int64_t rax_2;
    int64_t rdx_2;
    rax_2 = std::to_chars(&var_36, &var_26, arg2, 0x10);
    int64_t var_60 = rdx_2;
    char var_38;
    int32_t rdx_4 = rax_2 - &var_38;
    var_38 = 0x30;
    char var_37 = 0x78;
    
    if ((*(arg1 + 1) & 0x78) == 8)
    {
        char var_37_1 = 0x58;
        
        for (char (* i)[0x10] = &var_36; i != rax_2; i = &(*i)[1])
            *i = toupper(*i);
    }
    
    std::__format::_Sink_iter<char>* var_58 = nullptr;
    int64_t var_50 = 0;
    std::__format::_Sink_iter<char>* var_48;
    std::basic_string_view<char>::basic_string_view(&var_48, &var_38);
    var_58 = var_48;
    int64_t result;
    uint64_t var_40;
    std::__format::_Spec<char>* rdx;
    
    if (!(*arg1 & 0x40))
        result = std::__format::__write_padded_as_spec<char, std::__format::_Sink_iter<char> >(
            var_58, var_40, rdx_4, rdx, arg1);
    else
    {
        uint64_t rax_18 = std::__format::_Spec<char>::_M_get_width<std::basic_format_context<std::__format::_Sink_iter<char>, char> >(
            arg1);
        int64_t rax_19;
        rax_19 = std::basic_string_view<char>::size() >= rax_18;
        
        if (!rax_19)
        {
            var_48 = std::basic_format_context<std::__format::_Sink_iter<char>, char>::out();
            int64_t rax_25;
            int64_t rdx_12;
            rax_25 = std::basic_string_view<char>::substr(&var_58, 0);
            var_48 = std::__format::__write<std::__format::_Sink_iter<char>, char>(
                *std::move<std::__format::_Sink_iter<char>&>(&var_48), rax_25);
            std::basic_string_view<char>::remove_prefix(&var_58);
            result = std::__format::__write_padded<std::__format::_Sink_iter<char>, char>(
                *std::move<std::__format::_Sink_iter<char>&>(&var_48), var_58, var_40, 2, 
                rax_18 - rdx_4);
        }
        else
            result = std::__format::__write<std::__format::_Sink_iter<char>, char>(
                std::basic_format_context<std::__format::_Sink_iter<char>, char>::out(), var_58);
    }
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::formatter<int128_t, char>::format<std::__format::_Sink_iter<char> >(int128_t arg1, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg2)
{
    int64_t rdx;
    int64_t var_20 = rdx;
    int64_t rsi;
    return std::__format::__formatter_int<char>::format<int128_t, std::__format::_Sink_iter<char> >(
        rsi, arg2);
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::formatter<uint128_t, char>::format<std::__format::_Sink_iter<char> >(uint128_t arg1, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg2)
{
    int64_t rdx;
    int64_t var_20 = rdx;
    int64_t rsi;
    return 
        std::__format::__formatter_int<char>::format<uint128_t, std::__format::_Sink_iter<char> >(
        rsi, arg2);
}

int64_t std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::formatter<_Float128, char>::format<std::__format::_Sink_iter<char>>(_Float128, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const(std::basic_format_context<std::__format::_Sink_iter<char>, char>* arg1, int64_t arg2, int128_t arg3 @ zmm0)
{
    return std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<_Float128, std::__format::_Sink_iter<char>>(_Float128, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const(
        arg1, arg2, arg3);
}

char* std::span<char, 18446744073709551615ul>::end()
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t* rdi;
    int64_t var_30 = std::span<char, 18446744073709551615ul>::size() + *rdi;
    char* result;
    __normal_iterator<char*, std::span<char, 18446744073709551615ul> >::__normal_iterator(&result);
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

std::ranges::in_out_result<std::__conditional<borrowed_range<std::span<char, 18446744073709551615ul>&> >::type<(__begin, (declval<std::span<char, 18446744073709551615ul>&&>)), std::ranges::dangling>, std::__format::_Sink_iter<char> > std::ranges::__copy_fn::operator()<std::span<char, 18446744073709551615ul>&, std::__format::_Sink_iter<char> >(std::span<char, 18446744073709551615ul>&&& arg1, std::ranges::in_out_result<std::__conditional<borrowed_range<std::span<char, 18446744073709551615ul>&> >::type<(__begin, (declval<std::span<char, 18446744073709551615ul>&&>)), std::ranges::dangling>, std::__format::_Sink_iter<char> > arg2)
{
    int64_t var_28 = arg2;
    int64_t rdx;
    int64_t var_30 = rdx;
    std::__format::_Sink_iter<char>** rax = std::move<std::__format::_Sink_iter<char>&>(&var_30);
    int64_t rax_2 =
        std::ranges::__access::_End::operator()<std::span<char, 18446744073709551615ul>&>(
        &std::ranges::_Cpo::end);
    int64_t rax_4 =
        std::ranges::__access::_Begin::operator()<std::span<char, 18446744073709551615ul>&>(
        &std::ranges::_Cpo::begin);
    *rax;
    return std::ranges::__copy_fn::operator()<__normal_iterator<char*, std::span<char, 18446744073709551615ul> >, __normal_iterator<char*, std::span<char, 18446744073709551615ul> >, std::__format::_Sink_iter<char> >(
        arg1, rax_4, rax_2);
}

int64_t std::__format::_Sink<char>::_M_reset(std::span<char, 18446744073709551615ul> arg1, uint64_t arg2)
{
    void* var_30 = arg1;
    void* fsbase;
    int64_t rax_2 = *(fsbase + 0x28);
    *(var_30 + 8) = arg2;
    int64_t rdx;
    *(var_30 + 0x10) = rdx;
    char* var_28 = std::span<char, 18446744073709551615ul>::begin();
    *(var_30 + 0x18) =
        __normal_iterator<char*, std::span<char, 18446744073709551615ul> >::operator+(&var_28);
    
    if (rax_2 == *(fsbase + 0x28))
        return rax_2 - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

uint64_t std::string::resize(uint64_t arg1)
{
    char rsi;
    return std::string::resize(arg1, rsi);
}

int64_t std::string::empty()
{
    int64_t result;
    result = !std::string::size();
    return result;
}

void* std::__format::__formatter_int<char>::_M_parse<bool>(std::basic_format_parse_context<char>& arg1)
{
    int64_t rsi;
    void* result = std::__format::__formatter_int<char>::_M_do_parse(arg1, rsi);
    
    if (*(arg1 + 1) & 0x78 || (!(*arg1 & 0xc) && !(*arg1 & 0x10) && !(*arg1 & 0x40)))
        return result;
    
    std::__throw_format_error("format error: format-spec contai…");
    /* no return */
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_int<char>::format<std::__format::_Sink_iter<char> >(bool arg1, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg2)
{
    char* var_90 = arg1;
    char rax = arg2;
    void* fsbase;
    int64_t rax_1 = *(fsbase + 0x28);
    int64_t result;
    
    if ((var_90[1] & 0x78) == 0x38)
        result = std::__format::__formatter_int<char>::_M_format_character<std::__format::_Sink_iter<char> >(
            var_90, rax);
    else if (!(var_90[1] & 0x78))
    {
        std::string::string();
        std::basic_format_context<std::__format::_Sink_iter<char>, char>* var_78_1;
        char var_68[0x20];
        
        if (!(*var_90 & 0x20))
        {
            std::string::operator=(&var_68);
            var_78_1 = std::string::size();
        }
        else
        {
            std::basic_format_context<std::__format::_Sink_iter<char>, char>::locale();
            void var_80;
            std::use_facet<std::numpunct<char> >(&var_80);
            std::locale::~locale();
            
            if (!rax)
                std::numpunct<char>::falsename();
            else
                std::numpunct<char>::truename();
            
            std::string::operator=(&var_68);
            std::string::~string();
            var_78_1 = std::string::size();
        }
        int64_t rax_19;
        uint64_t rdx_6;
        rax_19 = std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::operator std::basic_string_view<char, std::char_traits<char> >();
        std::__format::_Spec<char>* rdx;
        result = std::__format::__write_padded_as_spec<char, std::__format::_Sink_iter<char> >(
            rax_19, rdx_6, var_78_1, rdx, var_90);
        std::string::~string();
    }
    else
        result =
            std::__format::__formatter_int<char>::format<uint8_t, std::__format::_Sink_iter<char> >(
            var_90, rax);
    
    *(fsbase + 0x28);
    
    if (rax_1 == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

void sub_41391d(void* arg1 @ rbp) __noreturn
{
    std::string::~string();
    void* fsbase;
    
    if (*(arg1 - 0x18) != *(fsbase + 0x28))
    {
        __stack_chk_fail();
        /* no return */
    }
    
    _Unwind_Resume();
    __stack_chk_fail();
    /* no return */
}

void* std::__format::__formatter_int<char>::_M_parse<char>(std::basic_format_parse_context<char>& arg1)
{
    int64_t rsi;
    void* result = std::__format::__formatter_int<char>::_M_do_parse(arg1, rsi);
    
    if ((*(arg1 + 1) & 0x78) != 0x38 && (*(arg1 + 1) & 0x78) != 0x78)
        return result;
    
    if (!(*arg1 & 0xc) && !(*arg1 & 0x10) && !(*arg1 & 0x40))
        return result;
    
    std::__throw_format_error("format error: format-spec contai…");
    /* no return */
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_int<char>::_M_format_character<std::__format::_Sink_iter<char> >(char arg1, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg2)
{
    int64_t var_40 = arg1;
    char var_44 = arg2;
    void* fsbase;
    int64_t rax_1 = *(fsbase + 0x28);
    int64_t var_38;
    std::basic_string_view<char>::basic_string_view(&var_38, &var_44);
    uint64_t var_30;
    std::__format::_Spec<char>* rdx;
    int64_t result = std::__format::__write_padded_as_spec<char, std::__format::_Sink_iter<char> >(
        var_38, var_30, 1, rdx, var_40);
    *(fsbase + 0x28);
    
    if (rax_1 == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_int<char>::format<uint8_t, std::__format::_Sink_iter<char> >(uint8_t arg1, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg2)
{
    char* var_a0 = arg1;
    uint8_t rax = arg2;
    void* fsbase;
    int64_t var_20 = *(fsbase + 0x28);
    int64_t result;
    
    if ((var_a0[1] & 0x78) != 0x38)
    {
        int64_t s;
        __builtin_memset(&s, 0, 0x20);
        void var_28;
        void* i_1 = &var_28;
        int64_t* var_78_1 = &var_20;
        void* i_2 = i_1;
        uint32_t rax_13 = var_a0[1] >> 3 & 0xf;
        uint64_t var_48;
        std::basic_format_context<std::__format::_Sink_iter<char>, char>* var_40;
        
        if (rax_13 <= 6)
        {
            if (rax_13 >= 5)
            {
                var_a0[1];
                std::basic_string_view<char>::basic_string_view(&var_48);
                uint64_t var_58_3 = var_48;
                std::basic_format_context<std::__format::_Sink_iter<char>, char>* var_50_3 = var_40;
                int64_t s_4;
                int64_t rdx_12;
                s_4 = std::to_chars(i_1, var_78_1, rax, 0x10);
                s = s_4;
                int64_t var_60_4 = rdx_12;
                
                if ((var_a0[1] & 0x78) == 0x30)
                {
                    for (void* i = i_1; i != s; i += 1)
                        *i = toupper(*i);
                }
            }
            else if (rax_13 == 4)
            {
                if (rax)
                {
                    std::basic_string_view<char>::basic_string_view(&var_48);
                    uint64_t var_58_2 = var_48;
                    std::basic_format_context<std::__format::_Sink_iter<char>, char>* var_50_2 =
                        var_40;
                }
                
                int64_t s_3;
                int64_t rdx_9;
                s_3 = std::to_chars(i_1, var_78_1, rax, 8);
                s = s_3;
                int64_t var_60_3 = rdx_9;
            }
            else if (rax_13 <= 4)
            {
                if (rax_13 > 1)
                {
                    if (rax_13 - 2 <= 1)
                    {
                        var_a0[1];
                        std::basic_string_view<char>::basic_string_view(&var_48);
                        uint64_t var_58_1 = var_48;
                        std::basic_format_context<std::__format::_Sink_iter<char>, char>* 
                            var_50_1 = var_40;
                        int64_t s_1;
                        int64_t rdx_4;
                        s_1 = std::to_chars(i_1, var_78_1, rax, 2);
                        s = s_1;
                        int64_t var_60_1 = rdx_4;
                    }
                }
                else if (rax_13 >= 0)
                {
                    int64_t s_2;
                    int64_t rdx_6;
                    s_2 = std::to_chars(i_1, var_78_1, rax, 0xa);
                    s = s_2;
                    int64_t var_60_2 = rdx_6;
                }
            }
        }
        
        char rax_44;
        
        if (!(*var_a0 & 0x10))
            rax_44 = 0;
        else if (!std::basic_string_view<char>::size())
            rax_44 = 0;
        else
            rax_44 = 1;
        
        if (rax_44)
        {
            i_1 += -(std::basic_string_view<char>::size());
            int64_t rax_47 = std::basic_string_view<char>::size();
            memcpy(i_1, std::basic_string_view<char>::data(), rax_47, i_1);
        }
        
        std::basic_string_view<char>::basic_string_view(&var_48, 
            std::__format::__put_sign<uint8_t>(rax, *var_a0 >> 2 & 3, i_1 - 1));
        result =
            std::__format::__formatter_int<char>::_M_format_int<std::__format::_Sink_iter<char> >(
            var_a0, var_48, var_40);
    }
    else
        result = std::__format::__formatter_int<char>::_M_format_character<std::__format::_Sink_iter<char> >(
            var_a0, std::__format::__formatter_int<char>::_S_to_character<uint8_t>(rax));
    
    *(fsbase + 0x28);
    
    if (var_20 == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

void* std::__format::__formatter_int<char>::_M_parse<int32_t>(std::basic_format_parse_context<char>& arg1)
{
    int64_t rsi;
    return std::__format::__formatter_int<char>::_M_do_parse(arg1, rsi);
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_int<char>::format<int32_t, std::__format::_Sink_iter<char> >(int32_t arg1, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg2)
{
    char* var_b0 = arg1;
    int32_t var_b4 = arg2;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t result;
    
    if ((var_b0[1] & 0x78) != 0x38)
    {
        int64_t s;
        __builtin_memset(&s, 0, 0x20);
        int32_t var_9c_1;
        
        if (var_b4 >= 0)
            var_9c_1 = var_b4;
        else
            var_9c_1 = -(var_b4);
        
        void var_45;
        void* i_1 = &var_45;
        void var_25;
        void* var_88_1 = &var_25;
        void* i_2 = i_1;
        uint32_t rax_14 = var_b0[1] >> 3 & 0xf;
        uint64_t var_58;
        std::basic_format_context<std::__format::_Sink_iter<char>, char>* var_50;
        
        if (rax_14 <= 6)
        {
            if (rax_14 >= 5)
            {
                var_b0[1];
                std::basic_string_view<char>::basic_string_view(&var_58);
                uint64_t var_68_3 = var_58;
                std::basic_format_context<std::__format::_Sink_iter<char>, char>* var_60_3 = var_50;
                int64_t s_4;
                int64_t rdx_12;
                s_4 = std::to_chars(i_1, var_88_1, var_9c_1, 0x10);
                s = s_4;
                int64_t var_70_4 = rdx_12;
                
                if ((var_b0[1] & 0x78) == 0x30)
                {
                    for (void* i = i_1; i != s; i += 1)
                        *i = toupper(*i);
                }
            }
            else if (rax_14 == 4)
            {
                if (var_b4)
                {
                    std::basic_string_view<char>::basic_string_view(&var_58);
                    uint64_t var_68_2 = var_58;
                    std::basic_format_context<std::__format::_Sink_iter<char>, char>* var_60_2 =
                        var_50;
                }
                
                int64_t s_3;
                int64_t rdx_9;
                s_3 = std::to_chars(i_1, var_88_1, var_9c_1, 8);
                s = s_3;
                int64_t var_70_3 = rdx_9;
            }
            else if (rax_14 <= 4)
            {
                if (rax_14 > 1)
                {
                    if (rax_14 - 2 <= 1)
                    {
                        var_b0[1];
                        std::basic_string_view<char>::basic_string_view(&var_58);
                        uint64_t var_68_1 = var_58;
                        std::basic_format_context<std::__format::_Sink_iter<char>, char>* 
                            var_60_1 = var_50;
                        int64_t s_1;
                        int64_t rdx_4;
                        s_1 = std::to_chars(i_1, var_88_1, var_9c_1, 2);
                        s = s_1;
                        int64_t var_70_1 = rdx_4;
                    }
                }
                else if (rax_14 >= 0)
                {
                    int64_t s_2;
                    int64_t rdx_6;
                    s_2 = std::to_chars(i_1, var_88_1, var_9c_1, 0xa);
                    s = s_2;
                    int64_t var_70_2 = rdx_6;
                }
            }
        }
        
        char rax_45;
        
        if (!(*var_b0 & 0x10))
            rax_45 = 0;
        else if (!std::basic_string_view<char>::size())
            rax_45 = 0;
        else
            rax_45 = 1;
        
        if (rax_45)
        {
            i_1 += -(std::basic_string_view<char>::size());
            int64_t rax_48 = std::basic_string_view<char>::size();
            memcpy(i_1, std::basic_string_view<char>::data(), rax_48, i_1);
        }
        
        std::basic_string_view<char>::basic_string_view(&var_58, 
            std::__format::__put_sign<int32_t>(var_b4, *var_b0 >> 2 & 3, i_1 - 1));
        result =
            std::__format::__formatter_int<char>::_M_format_int<std::__format::_Sink_iter<char> >(
            var_b0, var_58, var_50);
    }
    else
        result = std::__format::__formatter_int<char>::_M_format_character<std::__format::_Sink_iter<char> >(
            var_b0, std::__format::__formatter_int<char>::_S_to_character<int32_t>(var_b4));
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

void* std::__format::__formatter_int<char>::_M_parse<uint32_t>(std::basic_format_parse_context<char>& arg1)
{
    int64_t rsi;
    return std::__format::__formatter_int<char>::_M_do_parse(arg1, rsi);
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_int<char>::format<uint32_t, std::__format::_Sink_iter<char> >(uint32_t arg1, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg2)
{
    char* var_b0 = arg1;
    uint32_t var_b4 = arg2;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t result;
    
    if ((var_b0[1] & 0x78) != 0x38)
    {
        int64_t s;
        __builtin_memset(&s, 0, 0x20);
        void var_45;
        void* i_1 = &var_45;
        void var_25;
        void* var_88_1 = &var_25;
        void* i_2 = i_1;
        uint32_t rax_12 = var_b0[1] >> 3 & 0xf;
        uint64_t var_58;
        std::basic_format_context<std::__format::_Sink_iter<char>, char>* var_50;
        
        if (rax_12 <= 6)
        {
            if (rax_12 >= 5)
            {
                var_b0[1];
                std::basic_string_view<char>::basic_string_view(&var_58);
                uint64_t var_68_3 = var_58;
                std::basic_format_context<std::__format::_Sink_iter<char>, char>* var_60_3 = var_50;
                int64_t s_4;
                int64_t rdx_12;
                s_4 = std::to_chars(i_1, var_88_1, var_b4, 0x10);
                s = s_4;
                int64_t var_70_4 = rdx_12;
                
                if ((var_b0[1] & 0x78) == 0x30)
                {
                    for (void* i = i_1; i != s; i += 1)
                        *i = toupper(*i);
                }
            }
            else if (rax_12 == 4)
            {
                if (var_b4)
                {
                    std::basic_string_view<char>::basic_string_view(&var_58);
                    uint64_t var_68_2 = var_58;
                    std::basic_format_context<std::__format::_Sink_iter<char>, char>* var_60_2 =
                        var_50;
                }
                
                int64_t s_3;
                int64_t rdx_9;
                s_3 = std::to_chars(i_1, var_88_1, var_b4, 8);
                s = s_3;
                int64_t var_70_3 = rdx_9;
            }
            else if (rax_12 <= 4)
            {
                if (rax_12 > 1)
                {
                    if (rax_12 - 2 <= 1)
                    {
                        var_b0[1];
                        std::basic_string_view<char>::basic_string_view(&var_58);
                        uint64_t var_68_1 = var_58;
                        std::basic_format_context<std::__format::_Sink_iter<char>, char>* 
                            var_60_1 = var_50;
                        int64_t s_1;
                        int64_t rdx_4;
                        s_1 = std::to_chars(i_1, var_88_1, var_b4, 2);
                        s = s_1;
                        int64_t var_70_1 = rdx_4;
                    }
                }
                else if (rax_12 >= 0)
                {
                    int64_t s_2;
                    int64_t rdx_6;
                    s_2 = std::to_chars(i_1, var_88_1, var_b4, 0xa);
                    s = s_2;
                    int64_t var_70_2 = rdx_6;
                }
            }
        }
        
        char rax_43;
        
        if (!(*var_b0 & 0x10))
            rax_43 = 0;
        else if (!std::basic_string_view<char>::size())
            rax_43 = 0;
        else
            rax_43 = 1;
        
        if (rax_43)
        {
            i_1 += -(std::basic_string_view<char>::size());
            int64_t rax_46 = std::basic_string_view<char>::size();
            memcpy(i_1, std::basic_string_view<char>::data(), rax_46, i_1);
        }
        
        std::basic_string_view<char>::basic_string_view(&var_58, 
            std::__format::__put_sign<uint32_t>(var_b4, *var_b0 >> 2 & 3, i_1 - 1));
        result =
            std::__format::__formatter_int<char>::_M_format_int<std::__format::_Sink_iter<char> >(
            var_b0, var_58, var_50);
    }
    else
        result = std::__format::__formatter_int<char>::_M_format_character<std::__format::_Sink_iter<char> >(
            var_b0, std::__format::__formatter_int<char>::_S_to_character<uint32_t>(var_b4));
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

void* std::__format::__formatter_int<char>::_M_parse<int64_t>(std::basic_format_parse_context<char>& arg1)
{
    int64_t rsi;
    return std::__format::__formatter_int<char>::_M_do_parse(arg1, rsi);
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_int<char>::format<int64_t, std::__format::_Sink_iter<char> >(int64_t arg1, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t result;
    
    if ((*(arg1 + 1) & 0x78) != 0x38)
    {
        int64_t s;
        __builtin_memset(&s, 0, 0x20);
        int64_t var_c0_1;
        
        if (arg2 >= 0)
            var_c0_1 = arg2;
        else
            var_c0_1 = -(arg2);
        
        void var_65;
        void* i_1 = &var_65;
        void var_25;
        void* var_a8_1 = &var_25;
        void* i_2 = i_1;
        uint32_t rax_14 = *(arg1 + 1) >> 3 & 0xf;
        uint64_t var_78;
        std::basic_format_context<std::__format::_Sink_iter<char>, char>* var_70;
        
        if (rax_14 <= 6)
        {
            if (rax_14 >= 5)
            {
                *(arg1 + 1);
                std::basic_string_view<char>::basic_string_view(&var_78);
                uint64_t var_88_3 = var_78;
                std::basic_format_context<std::__format::_Sink_iter<char>, char>* var_80_3 = var_70;
                int64_t s_4;
                int64_t rdx_12;
                s_4 = std::to_chars(i_1, var_a8_1, var_c0_1, 0x10);
                s = s_4;
                int64_t var_90_4 = rdx_12;
                
                if ((*(arg1 + 1) & 0x78) == 0x30)
                {
                    for (void* i = i_1; i != s; i += 1)
                        *i = toupper(*i);
                }
            }
            else if (rax_14 == 4)
            {
                if (arg2)
                {
                    std::basic_string_view<char>::basic_string_view(&var_78);
                    uint64_t var_88_2 = var_78;
                    std::basic_format_context<std::__format::_Sink_iter<char>, char>* var_80_2 =
                        var_70;
                }
                
                int64_t s_3;
                int64_t rdx_9;
                s_3 = std::to_chars(i_1, var_a8_1, var_c0_1, 8);
                s = s_3;
                int64_t var_90_3 = rdx_9;
            }
            else if (rax_14 <= 4)
            {
                if (rax_14 > 1)
                {
                    if (rax_14 - 2 <= 1)
                    {
                        *(arg1 + 1);
                        std::basic_string_view<char>::basic_string_view(&var_78);
                        uint64_t var_88_1 = var_78;
                        std::basic_format_context<std::__format::_Sink_iter<char>, char>* 
                            var_80_1 = var_70;
                        int64_t s_1;
                        int64_t rdx_4;
                        s_1 = std::to_chars(i_1, var_a8_1, var_c0_1, 2);
                        s = s_1;
                        int64_t var_90_1 = rdx_4;
                    }
                }
                else if (rax_14 >= 0)
                {
                    int64_t s_2;
                    int64_t rdx_6;
                    s_2 = std::to_chars(i_1, var_a8_1, var_c0_1, 0xa);
                    s = s_2;
                    int64_t var_90_2 = rdx_6;
                }
            }
        }
        
        int64_t rax_44;
        
        if (*arg1 & 0x10)
            rax_44 = std::basic_string_view<char>::size();
        
        char rax_45;
        
        if (!(*arg1 & 0x10) || !rax_44)
            rax_45 = 0;
        else
            rax_45 = 1;
        
        if (rax_45)
        {
            i_1 += -(std::basic_string_view<char>::size());
            int64_t rax_48 = std::basic_string_view<char>::size();
            memcpy(i_1, std::basic_string_view<char>::data(), rax_48, i_1);
        }
        
        std::basic_string_view<char>::basic_string_view(&var_78, 
            std::__format::__put_sign<int64_t>(arg2, *arg1 >> 2 & 3, i_1 - 1));
        result =
            std::__format::__formatter_int<char>::_M_format_int<std::__format::_Sink_iter<char> >(
            arg1, var_78, var_70);
    }
    else
        result = std::__format::__formatter_int<char>::_M_format_character<std::__format::_Sink_iter<char> >(
            arg1, std::__format::__formatter_int<char>::_S_to_character<int64_t>(arg2));
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

void* std::__format::__formatter_int<char>::_M_parse<uint64_t>(std::basic_format_parse_context<char>& arg1)
{
    int64_t rsi;
    return std::__format::__formatter_int<char>::_M_do_parse(arg1, rsi);
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_int<char>::format<uint64_t, std::__format::_Sink_iter<char> >(uint64_t arg1, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t result;
    
    if ((*(arg1 + 1) & 0x78) != 0x38)
    {
        int64_t s;
        __builtin_memset(&s, 0, 0x20);
        void var_65;
        void* i_1 = &var_65;
        void var_25;
        void* var_a8_1 = &var_25;
        void* i_2 = i_1;
        uint32_t rax_12 = *(arg1 + 1) >> 3 & 0xf;
        uint64_t var_78;
        std::basic_format_context<std::__format::_Sink_iter<char>, char>* var_70;
        
        if (rax_12 <= 6)
        {
            if (rax_12 >= 5)
            {
                *(arg1 + 1);
                std::basic_string_view<char>::basic_string_view(&var_78);
                uint64_t var_88_3 = var_78;
                std::basic_format_context<std::__format::_Sink_iter<char>, char>* var_80_3 = var_70;
                int64_t s_4;
                int64_t rdx_12;
                s_4 = std::to_chars(i_1, var_a8_1, arg2, 0x10);
                s = s_4;
                int64_t var_90_4 = rdx_12;
                
                if ((*(arg1 + 1) & 0x78) == 0x30)
                {
                    for (void* i = i_1; i != s; i += 1)
                        *i = toupper(*i);
                }
            }
            else if (rax_12 == 4)
            {
                if (arg2)
                {
                    std::basic_string_view<char>::basic_string_view(&var_78);
                    uint64_t var_88_2 = var_78;
                    std::basic_format_context<std::__format::_Sink_iter<char>, char>* var_80_2 =
                        var_70;
                }
                
                int64_t s_3;
                int64_t rdx_9;
                s_3 = std::to_chars(i_1, var_a8_1, arg2, 8);
                s = s_3;
                int64_t var_90_3 = rdx_9;
            }
            else if (rax_12 <= 4)
            {
                if (rax_12 > 1)
                {
                    if (rax_12 - 2 <= 1)
                    {
                        *(arg1 + 1);
                        std::basic_string_view<char>::basic_string_view(&var_78);
                        uint64_t var_88_1 = var_78;
                        std::basic_format_context<std::__format::_Sink_iter<char>, char>* 
                            var_80_1 = var_70;
                        int64_t s_1;
                        int64_t rdx_4;
                        s_1 = std::to_chars(i_1, var_a8_1, arg2, 2);
                        s = s_1;
                        int64_t var_90_1 = rdx_4;
                    }
                }
                else if (rax_12 >= 0)
                {
                    int64_t s_2;
                    int64_t rdx_6;
                    s_2 = std::to_chars(i_1, var_a8_1, arg2, 0xa);
                    s = s_2;
                    int64_t var_90_2 = rdx_6;
                }
            }
        }
        
        int64_t rax_42;
        
        if (*arg1 & 0x10)
            rax_42 = std::basic_string_view<char>::size();
        
        char rax_43;
        
        if (!(*arg1 & 0x10) || !rax_42)
            rax_43 = 0;
        else
            rax_43 = 1;
        
        if (rax_43)
        {
            i_1 += -(std::basic_string_view<char>::size());
            int64_t rax_46 = std::basic_string_view<char>::size();
            memcpy(i_1, std::basic_string_view<char>::data(), rax_46, i_1);
        }
        
        std::basic_string_view<char>::basic_string_view(&var_78, 
            std::__format::__put_sign<uint64_t>(arg2, *arg1 >> 2 & 3, i_1 - 1));
        result =
            std::__format::__formatter_int<char>::_M_format_int<std::__format::_Sink_iter<char> >(
            arg1, var_78, var_70);
    }
    else
        result = std::__format::__formatter_int<char>::_M_format_character<std::__format::_Sink_iter<char> >(
            arg1, std::__format::__formatter_int<char>::_S_to_character<uint64_t>(arg2));
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

int64_t* std::__format::__formatter_fp<char>::parse(std::basic_format_parse_context<char>&)::'lambda'()::operator()()
{
    int64_t* rdi;
    int64_t* rdx = rdi[1];
    int64_t* result = *rdi;
    *result = *rdx;
    result[1] = rdx[1];
    return result;
}

int64_t std::__format::__formatter_fp<char>::parse(std::basic_format_parse_context<char>&)::'lambda0'()::operator()()
{
    int64_t* rdi;
    
    if (**rdi != *rdi[1] && ***rdi != 0x7d)
        return 0;
    
    rdi[2];
    std::__format::__formatter_fp<char>::parse(std::basic_format_parse_context<char>&)::'lambda'()::operator()();
    return 1;
}

void* std::__format::__formatter_fp<char>::parse(std::basic_format_parse_context<char>& arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_44 = 0;
    int32_t var_3c = 0;
    int32_t var_3c_1 = 0x20;
    std::basic_format_parse_context<char>* var_58 = std::basic_format_parse_context<char>::end();
    void* result_1 = std::basic_format_parse_context<char>::begin();
    std::basic_format_parse_context<char>& var_38 = arg1;
    int64_t* var_30 = &var_44;
    int64_t* var_28 = &result_1;
    int64_t* var_20 = &var_58;
    std::basic_format_parse_context<char>&* var_18 = &var_38;
    void* result;
    
    if (!std::__format::__formatter_fp<char>::parse(std::basic_format_parse_context<char>&)::'lambda0'()::operator()())
    {
        result_1 = std::__format::_Spec<char>::_M_parse_fill_and_align(&var_44, result_1);
        
        if (!std::__format::__formatter_fp<char>::parse(std::basic_format_parse_context<char>&)::'lambda0'()::operator()())
        {
            result_1 = std::__format::_Spec<char>::_M_parse_sign(&var_44, result_1);
            
            if (!std::__format::__formatter_fp<char>::parse(std::basic_format_parse_context<char>&)::'lambda0'()::operator()())
            {
                result_1 = std::__format::_Spec<char>::_M_parse_alternate_form(&var_44, result_1);
                
                if (!std::__format::__formatter_fp<char>::parse(std::basic_format_parse_context<char>&)::'lambda0'()::operator()())
                {
                    result_1 = std::__format::_Spec<char>::_M_parse_zero_fill(&var_44, result_1);
                    
                    if (std::__format::__formatter_fp<char>::parse(std::basic_format_parse_context<char>&)::'lambda0'()::operator()())
                        result = result_1;
                    else if (*result_1 == 0x2e)
                    {
                        label_4151be:
                        result_1 = std::__format::_Spec<char>::_M_parse_precision(&var_44, 
                            result_1, var_58);
                        
                        if (!std::__format::__formatter_fp<char>::parse(std::basic_format_parse_context<char>&)::'lambda0'()::operator()())
                        {
                            result_1 =
                                std::__format::_Spec<char>::_M_parse_locale(&var_44, result_1);
                            
                            if (!std::__format::__formatter_fp<char>::parse(std::basic_format_parse_context<char>&)::'lambda0'()::operator()())
                            {
                                switch (*result_1 - 0x41)
                                {
                                    case 0:
                                    {
                                        *var_44[1] = (*var_44[1] & 0x87) | 0x10;
                                        result_1 += 1;
                                        break;
                                    }
                                    case 4:
                                    {
                                        *var_44[1] = (*var_44[1] & 0x87) | 0x20;
                                        result_1 += 1;
                                        break;
                                    }
                                    case 5:
                                    {
                                        *var_44[1] = (*var_44[1] & 0x87) | 0x30;
                                        result_1 += 1;
                                        break;
                                    }
                                    case 6:
                                    {
                                        *var_44[1] = (*var_44[1] & 0x87) | 0x40;
                                        result_1 += 1;
                                        break;
                                    }
                                    case 0x20:
                                    {
                                        *var_44[1] = (*var_44[1] & 0x87) | 8;
                                        result_1 += 1;
                                        break;
                                    }
                                    case 0x24:
                                    {
                                        *var_44[1] = (*var_44[1] & 0x87) | 0x18;
                                        result_1 += 1;
                                        break;
                                    }
                                    case 0x25:
                                    {
                                        *var_44[1] = (*var_44[1] & 0x87) | 0x28;
                                        result_1 += 1;
                                        break;
                                    }
                                    case 0x26:
                                    {
                                        *var_44[1] = (*var_44[1] & 0x87) | 0x38;
                                        result_1 += 1;
                                        break;
                                    }
                                }
                                
                                if (!std::__format::__formatter_fp<char>::parse(std::basic_format_parse_context<char>&)::'lambda0'()::operator()())
                                {
                                    *(fsbase + 0x28);
                                    
                                    if (rax != *(fsbase + 0x28))
                                    {
                                        __stack_chk_fail();
                                        /* no return */
                                    }
                                    
                                    std::__format::__failed_to_parse_format_spec();
                                    /* no return */
                                }
                                
                                result = result_1;
                            }
                            else
                                result = result_1;
                        }
                        else
                            result = result_1;
                    }
                    else
                    {
                        result_1 =
                            std::__format::_Spec<char>::_M_parse_width(&var_44, result_1, var_58);
                        
                        if (!std::__format::__formatter_fp<char>::parse(std::basic_format_parse_context<char>&)::'lambda0'()::operator()())
                            goto label_4151be;
                        
                        result = result_1;
                    }
                }
                else
                    result = result_1;
            }
            else
                result = result_1;
        }
        else
            result = result_1;
    }
    else
        result = result_1;
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<float, std::__format::_Sink_iter<char>>(float, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda'(char*, char*)::operator()(char*, char*) const(int64_t* arg1, char* arg2, char* arg3)
{
    if (**arg1)
        return std::to_chars(arg2, arg3, *arg1[1], *arg1[2], *arg1[3]);
    
    if (!*arg1[2])
        return std::to_chars(arg2, arg3, *arg1[1]);
    
    return std::to_chars(arg2, arg3, *arg1[1], *arg1[2]);
}

void* std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<float, std::__format::_Sink_iter<char>>(float, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)::operator()(char*, unsigned long) const(int64_t* arg1, void* arg2, int64_t arg3)
{
    int64_t* rbx = arg1[1];
    int64_t rax_2;
    int64_t rdx_3;
    rax_2 = std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<float, std::__format::_Sink_iter<char>>(float, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda'(char*, char*)::operator()(char*, char*) const(
        *arg1, arg2 + 1, arg2 + arg3 - 1);
    *rbx = rax_2;
    rbx[1] = rdx_3;
    
    if (*(arg1[1] + 8))
        return nullptr;
    
    return *arg1[1] - arg2;
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<float, std::__format::_Sink_iter<char> >(float arg1, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg2)
{
    float var_204 = arg1;
    void* fsbase;
    int64_t var_20 = *(fsbase + 0x28);
    std::string::string();
    int64_t var_168 = 0;
    int64_t var_160 = 0;
    int64_t var_1e0 = 6;
    
    if (*(arg2 + 1) & 6)
        uint64_t var_1e0_1 = std::__format::_Spec<char>::_M_get_precision<std::basic_format_context<std::__format::_Sink_iter<char>, char> >(
            arg2);
    
    void var_a7;
    void* var_1d0 = &var_a7;
    void var_28;
    void* var_1c8 = &var_28;
    int32_t var_1f0 = 0;
    char var_1f5 = 0;
    char var_1f4 = 0;
    char var_1f3 = 0x65;
    /* jump -> *(((*(arg2 + 1) >> 3 & 0xf) << 2) + &data_4162f0) + &data_4162f0 */
}

int64_t std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<double, std::__format::_Sink_iter<char>>(double, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda'(char*, char*)::operator()(char*, char*) const(int64_t* arg1, char* arg2, char* arg3)
{
    if (**arg1)
        return std::to_chars(arg2, arg3, *arg1[1], *arg1[2], *arg1[3]);
    
    if (!*arg1[2])
        return std::to_chars(arg2, arg3, *arg1[1]);
    
    return std::to_chars(arg2, arg3, *arg1[1], *arg1[2]);
}

void* std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<double, std::__format::_Sink_iter<char>>(double, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)::operator()(char*, unsigned long) const(int64_t* arg1, void* arg2, int64_t arg3)
{
    int64_t* rbx = arg1[1];
    int64_t rax_2;
    int64_t rdx_3;
    rax_2 = std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<double, std::__format::_Sink_iter<char>>(double, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda'(char*, char*)::operator()(char*, char*) const(
        *arg1, arg2 + 1, arg2 + arg3 - 1);
    *rbx = rax_2;
    rbx[1] = rdx_3;
    
    if (*(arg1[1] + 8))
        return nullptr;
    
    return *arg1[1] - arg2;
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<double, std::__format::_Sink_iter<char> >(double arg1, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg2)
{
    double var_208 = arg1;
    void* fsbase;
    int64_t var_20 = *(fsbase + 0x28);
    std::string::string();
    int64_t var_168 = 0;
    int64_t var_160 = 0;
    int64_t var_1e0 = 6;
    
    if (*(arg2 + 1) & 6)
        uint64_t var_1e0_1 = std::__format::_Spec<char>::_M_get_precision<std::basic_format_context<std::__format::_Sink_iter<char>, char> >(
            arg2);
    
    void var_a7;
    void* var_1d0 = &var_a7;
    void var_28;
    void* var_1c8 = &var_28;
    int32_t var_1f0 = 0;
    char var_1f5 = 0;
    char var_1f4 = 0;
    char var_1f3 = 0x65;
    /* jump -> *(((*(arg2 + 1) >> 3 & 0xf) << 2) + &data_417210) + &data_417210 */
}

int64_t std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<long double, std::__format::_Sink_iter<char>>(long double, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda'(char*, char*)::operator()(char*, char*) const(int64_t* arg1, char* arg2, char* arg3)
{
    long double zmm0;
    
    if (**arg1)
    {
        int80_t var_38 = *arg1[1];
        return std::to_chars(arg2, arg3, zmm0, *arg1[2], *arg1[3]);
    }
    
    if (!*arg1[2])
    {
        int80_t var_38_2 = *arg1[1];
        return std::to_chars(arg2, arg3, zmm0);
    }
    
    int80_t var_38_1 = *arg1[1];
    return std::to_chars(arg2, arg3, zmm0, *arg1[2]);
}

void* std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<long double, std::__format::_Sink_iter<char>>(long double, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)::operator()(char*, unsigned long) const(int64_t* arg1, void* arg2, int64_t arg3)
{
    int64_t* rbx = arg1[1];
    int64_t rax_2;
    int64_t rdx_3;
    rax_2 = std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<long double, std::__format::_Sink_iter<char>>(long double, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda'(char*, char*)::operator()(char*, char*) const(
        *arg1, arg2 + 1, arg2 + arg3 - 1);
    *rbx = rax_2;
    rbx[1] = rdx_3;
    
    if (*(arg1[1] + 8))
        return nullptr;
    
    return *arg1[1] - arg2;
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<long double, std::__format::_Sink_iter<char> >(long double arg1, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg2)
{
    void* fsbase;
    int64_t var_20 = *(fsbase + 0x28);
    std::string::string();
    int64_t var_168 = 0;
    int64_t var_160 = 0;
    int64_t var_1e0 = 6;
    
    if (*(arg2 + 1) & 6)
        uint64_t var_1e0_1 = std::__format::_Spec<char>::_M_get_precision<std::basic_format_context<std::__format::_Sink_iter<char>, char> >(
            arg2);
    
    void var_a7;
    void* var_1d0 = &var_a7;
    void var_28;
    void* var_1c8 = &var_28;
    int32_t var_1f0 = 0;
    char var_1f5 = 0;
    char var_1f4 = 0;
    char var_1f3 = 0x65;
    /* jump -> *(((*(arg2 + 1) >> 3 & 0xf) << 2) + &data_418100) + &data_418100 */
}

std::__format::_Sink_iter<char> std::__format::__formatter_str<char>::format<std::__format::_Sink_iter<char> >(std::basic_string_view<char> arg1, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg2)
{
    int16_t* var_20 = arg1;
    std::basic_format_context<std::__format::_Sink_iter<char>, char>& var_38 = arg2;
    
    if (!(*var_20 & 0x180) && !(*(var_20 + 1) & 6))
        return std::__format::__write<std::__format::_Sink_iter<char>, char>(
            std::basic_format_context<std::__format::_Sink_iter<char>, char>::out(), var_38);
    
    int64_t var_18_1;
    
    if (!(*(var_20 + 1) & 6))
        var_18_1 = std::__unicode::__field_width<char>(var_38);
    else
        var_18_1 = std::__unicode::__truncate<char>(&var_38, 
            std::__format::_Spec<char>::_M_get_precision<std::basic_format_context<std::__format::_Sink_iter<char>, char> >(
                var_20));
    
    std::__format::_Spec<char>* rcx;
    uint64_t rdx;
    return std::__format::__write_padded_as_spec<char, std::__format::_Sink_iter<char> >(var_38, 
        rdx, var_18_1, rcx, var_20);
}

uint64_t std::__format::_Spec<char>::_M_get_width<std::basic_format_context<std::__format::_Sink_iter<char>, char> >(std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    uint64_t result = 0;
    
    if ((*arg1 & 0x180) == 0x80)
        result = *(arg1 + 4);
    else if ((*arg1 & 0x180) == 0x100)
    {
        *(arg1 + 4);
        std::basic_format_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char> > 
            var_38;
        std::basic_format_context<std::__format::_Sink_iter<char>, char>::arg(&var_38);
        result = std::__format::__int_from_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char> >(
            &var_38);
    }
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

uint64_t std::basic_string_view<char>::remove_prefix(uint64_t arg1)
{
    int64_t rsi;
    *(arg1 + 8) += rsi;
    *arg1 -= rsi;
    return arg1;
}

void std::__format::_Sink_iter<char> std::__format::__write_padded<std::__format::_Sink_iter<char>, char>(std::__format::_Sink_iter<char>, std::basic_string_view<char, std::char_traits<char>>, std::__format::_Align, unsigned long, char32_t)::'lambda'(unsigned long, std::__format::_Sink_iter<char>&)::operator()(unsigned long, std::__format::_Sink_iter<char>&) const(int64_t* arg1, int64_t arg2, int64_t* arg3)
{
    int64_t var_38 = arg2;
    
    if (var_38)
    {
        while (true)
        {
            *arg1;
            int64_t* rax;
            rax = std::basic_string_view<char>::size() < var_38;
            
            if (!rax)
                break;
            
            int64_t* rbx_1 = *arg1;
            std::__format::_Sink_iter<char>** rax_3 =
                std::move<std::__format::_Sink_iter<char>&>(arg3);
            rbx_1[1];
            *arg3 = std::__format::__write<std::__format::_Sink_iter<char>, char>(*rax_3, *rbx_1);
            *arg1;
            var_38 -= std::basic_string_view<char>::size();
        }
        
        if (var_38)
        {
            int64_t rax_14;
            int64_t rdx_3;
            rax_14 = std::basic_string_view<char>::substr(*arg1, 0);
            *arg3 = std::__format::__write<std::__format::_Sink_iter<char>, char>(
                *std::move<std::__format::_Sink_iter<char>&>(arg3), rax_14);
        }
    }
}

auto std::ranges::__access::_Begin::operator()<char32_t const (&)[0x1]>(char32_t const (&&& arg1)[0x1]) __pure
{
    char32_t const (&&& var_10)[0x1] = arg1;
    int64_t result;
    return result;
}

auto std::ranges::__access::_End::operator()<char32_t const (&)[0x1]>(char32_t const (&&& arg1)[0x1]) __pure
{
    char32_t const (&&& var_10)[0x1] = arg1;
    int64_t rsi;
    return rsi + 4;
}

std::remove_reference<char32_t const (&)[0x1]>::type* std::__unicode::_Utf_view<char, char32_t const (&)[0x1]>::_Utf_view(char32_t const (& arg1)[0x1])
{
    std::remove_reference<char32_t const (&)[0x1]>::type* rsi;
    std::remove_reference<char32_t const (&)[0x1]>::type* result =
        std::forward<char32_t const (&)[0x1]>(rsi);
    *arg1 = result;
    return result;
}

std::remove_reference<char32_t const (&)[0x1]>::type& std::forward<char32_t const (&)[0x1]>(std::remove_reference<char32_t const (&)[0x1]>::type& arg1) __pure
{
    return arg1;
}

auto std::__unicode::_Utf_view<char, char32_t const (&)[0x1]>::_M_begin<char32_t const*, char32_t const*>(char32_t const* arg1, char32_t const* arg2)
{
    char32_t const* var_18 = arg2;
    char32_t* rdx;
    std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::_Utf_iterator(
        arg1, rdx, rdx);
    return arg1;
}

char32_t* std::__unicode::_Utf_view<char, char32_t const (&)[0x1]>::begin()
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t* rsi;
    *rsi;
    *rsi;
    char32_t* result;
    std::__unicode::_Utf_view<char, char32_t const (&)[0x1]>::_M_begin<char32_t const*, char32_t const*>(
        result, rsi);
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

auto std::__unicode::_Utf_view<char, char32_t const (&)[0x1]>::_M_end<char32_t const*, char32_t const*>(char32_t const* arg1, char32_t const* arg2)
{
    char32_t const* var_18 = arg2;
    char32_t* rcx;
    char32_t* rdx;
    std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::_Utf_iterator(
        arg1, rdx, rcx);
    return arg1;
}

char32_t* std::__unicode::_Utf_view<char, char32_t const (&)[0x1]>::end()
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t* rsi;
    *rsi;
    *rsi;
    char32_t* result;
    std::__unicode::_Utf_view<char, char32_t const (&)[0x1]>::_M_end<char32_t const*, char32_t const*>(
        result, rsi);
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

std::__format::_Sink_iter<char> std::__format::__write_padded<std::__format::_Sink_iter<char>, char>(std::__format::_Sink_iter<char> arg1, std::basic_string_view<char> arg2, std::__format::_Align arg3, uint64_t arg4, char32_t arg5)
{
    int64_t result_1 = arg1;
    int64_t rax = arg2;
    int64_t var_140 = arg3;
    int32_t var_134 = arg4;
    uint64_t var_150 = arg5;
    void* fsbase;
    int64_t rax_2 = *(fsbase + 0x28);
    int64_t var_f8 = 0x20;
    char var_48[0x28];
    var_48[0] = 0;
    int64_t var_e8;
    std::basic_string_view<char>::basic_string_view(&var_e8, &var_48);
    int64_t* var_120 = &var_e8;
    uint64_t var_110;
    uint64_t var_108;
    uint64_t var_100;
    
    if (var_134 == 3)
    {
        var_110 = var_150 >> 1;
        var_108 = var_110 + (var_150 & 1);
        var_100 = var_108;
    }
    else if (var_134 != 2)
    {
        var_110 = 0;
        var_108 = var_150;
        var_100 = var_108;
    }
    else
    {
        var_110 = var_150;
        var_108 = 0;
        var_100 = var_110;
    }
    
    int64_t result;
    char32_t r9;
    
    if (std::__unicode::__is_single_code_unit<char>(r9) == 1)
    {
        if (var_100 > 0x1f)
            var_100 = 0x20;
        else
            std::basic_string_view<char>::remove_suffix(&var_e8);
        
        std::char_traits<char>::assign(&var_48, var_100, r9);
        std::__format::_Sink_iter<char> std::__format::__write_padded<std::__format::_Sink_iter<char>, char>(std::__format::_Sink_iter<char>, std::basic_string_view<char, std::char_traits<char>>, std::__format::_Align, unsigned long, char32_t)::'lambda'(unsigned long, std::__format::_Sink_iter<char>&)::operator()(unsigned long, std::__format::_Sink_iter<char>&) const(
            &var_120, var_110, &result_1);
        result_1 = std::__format::__write<std::__format::_Sink_iter<char>, char>(
            *std::move<std::__format::_Sink_iter<char>&>(&result_1), rax);
        std::__format::_Sink_iter<char> std::__format::__write_padded<std::__format::_Sink_iter<char>, char>(std::__format::_Sink_iter<char>, std::basic_string_view<char, std::char_traits<char>>, std::__format::_Align, unsigned long, char32_t)::'lambda'(unsigned long, std::__format::_Sink_iter<char>&)::operator()(unsigned long, std::__format::_Sink_iter<char>&) const(
            &var_120, var_108, &result_1);
        result = result_1;
    }
    else
    {
        char32_t var_cc_1 = r9;
        char32_t const var_118[0x1];
        std::__unicode::_Utf_view<char, char32_t const (&)[0x1]>::_Utf_view(&var_118);
        void var_121;
        void* var_f0_1 = &var_121;
        std::__unicode::_Utf_view<char, char32_t const (&)[0x1]>::end();
        std::__unicode::_Utf_view<char, char32_t const (&)[0x1]>::begin();
        int64_t var_a8;
        int64_t var_180_1 = var_a8;
        int64_t var_a0;
        int64_t var_178_1 = var_a0;
        int64_t var_98;
        int64_t var_170_1 = var_98;
        int64_t var_90;
        int64_t var_168_1 = var_90;
        int64_t var_88;
        int64_t var_160_1 = var_88;
        int64_t var_78;
        int64_t var_1a8_1 = var_78;
        int64_t var_70;
        int64_t var_1a0_1 = var_70;
        int64_t var_68;
        int64_t var_198_1 = var_68;
        int64_t var_60;
        int64_t var_190_1 = var_60;
        int64_t var_58;
        int64_t var_188_1 = var_58;
        std::allocator<char> var_c8;
        std::string::string<std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>, void>(
            &var_c8, &var_121, &var_c8);
        int64_t rax_18;
        int64_t rdx_4;
        rax_18 = std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >::operator std::basic_string_view<char, std::char_traits<char> >();
        var_e8 = rax_18;
        int64_t var_e0_1 = rdx_4;
        
        while (true)
        {
            uint64_t rax_22 = var_110;
            var_110 = rax_22 - 1;
            rax_22 = rax_22;
            
            if (!rax_22)
                break;
            
            result_1 = std::__format::__write<std::__format::_Sink_iter<char>, char>(
                *std::move<std::__format::_Sink_iter<char>&>(&result_1), var_e8);
        }
        
        result_1 = std::__format::__write<std::__format::_Sink_iter<char>, char>(
            *std::move<std::__format::_Sink_iter<char>&>(&result_1), rax);
        
        while (true)
        {
            uint64_t rax_29 = var_108;
            var_108 = rax_29 - 1;
            rax_29 = rax_29;
            
            if (!rax_29)
                break;
            
            result_1 = std::__format::__write<std::__format::_Sink_iter<char>, char>(
                *std::move<std::__format::_Sink_iter<char>&>(&result_1), var_e8);
        }
        
        result = result_1;
        std::string::~string();
    }
    
    *(fsbase + 0x28);
    
    if (rax_2 == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

void sub_418a5c(void* arg1 @ rbp) __noreturn
{
    void* fsbase;
    
    if (*(arg1 - 0x18) != *(fsbase + 0x28))
    {
        __stack_chk_fail();
        /* no return */
    }
    
    int64_t rax = _Unwind_Resume();
    std::string::~string();
    
    if (*(arg1 - 0x18) != *(fsbase + 0x28))
    {
        __stack_chk_fail();
        /* no return */
    }
    
    _Unwind_Resume(rax);
    __stack_chk_fail();
    /* no return */
}

std::__format::_Sink_iter<char> std::__format::__write_padded_as_spec<char, std::__format::_Sink_iter<char> >(std::basic_string_view<std::type_identity<char>::type, std::char_traits<std::type_identity<char>::type> > arg1, uint64_t arg2, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg3, std::__format::_Spec<char> const& arg4, std::__format::_Align arg5)
{
    int64_t rax = arg1;
    std::basic_format_context<std::__format::_Sink_iter<char>, char>* var_50 = arg5;
    uint32_t r9;
    uint32_t var_54 = r9;
    uint64_t rax_2 = std::__format::_Spec<char>::_M_get_width<std::basic_format_context<std::__format::_Sink_iter<char>, char> >(
        var_50);
    
    if (arg3 >= rax_2)
        return std::__format::__write<std::__format::_Sink_iter<char>, char>(
            std::basic_format_context<std::__format::_Sink_iter<char>, char>::out(), rax);
    
    if (*var_50 & 3)
        var_54 = *var_50 & 3;
    
    *(var_50 + 8);
    return std::__format::__write_padded<std::__format::_Sink_iter<char>, char>(
        std::basic_format_context<std::__format::_Sink_iter<char>, char>::out(), rax, arg2, var_54, 
        rax_2 - arg3);
}

void* std::__format::__formatter_int<char>::_M_parse<int128_t>(std::basic_format_parse_context<char>& arg1)
{
    int64_t rsi;
    return std::__format::__formatter_int<char>::_M_do_parse(arg1, rsi);
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_int<char>::format<int128_t, std::__format::_Sink_iter<char> >(int128_t arg1, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg2)
{
    void* fsbase;
    int64_t rax_2 = *(fsbase + 0x28);
    int64_t result;
    int64_t rsi;
    
    if ((*(arg2 + 1) & 0x78) != 0x38)
    {
        int64_t s;
        __builtin_memset(&s, 0, 0x20);
        int32_t var_b8_1;
        char* rdx;
        int64_t var_b0_1;
        
        if (rdx >= 0)
        {
            var_b8_1 = rsi;
            var_b0_1 = rdx;
        }
        else
        {
            var_b8_1 = -(rsi);
            var_b0_1 = -((rdx + 0));
        }
        void var_a5;
        void* i_1 = &var_a5;
        void var_25;
        void* var_f8_1 = &var_25;
        void* i_2 = i_1;
        uint32_t rax_16 = *(arg2 + 1) >> 3 & 0xf;
        uint64_t var_c8;
        std::basic_format_context<std::__format::_Sink_iter<char>, char>* var_c0;
        
        if (rax_16 <= 6)
        {
            if (rax_16 >= 5)
            {
                *(arg2 + 1);
                std::basic_string_view<char>::basic_string_view(&var_c8);
                uint64_t var_d8_3 = var_c8;
                std::basic_format_context<std::__format::_Sink_iter<char>, char>* var_d0_3 = var_c0;
                int64_t s_4;
                int64_t rdx_22;
                s_4 = std::to_chars(i_1, var_f8_1, rsi, var_b8_1);
                s = s_4;
                int64_t var_e0_4 = rdx_22;
                
                if ((*(arg2 + 1) & 0x78) == 0x30)
                {
                    for (void* i = i_1; i != s; i += 1)
                        *i = toupper(*i);
                }
            }
            else if (rax_16 == 4)
            {
                if (rsi | rdx)
                {
                    std::basic_string_view<char>::basic_string_view(&var_c8);
                    uint64_t var_d8_2 = var_c8;
                    std::basic_format_context<std::__format::_Sink_iter<char>, char>* var_d0_2 =
                        var_c0;
                }
                
                int64_t s_3;
                int64_t rdx_18;
                s_3 = std::to_chars(i_1, var_f8_1, rsi, var_b8_1);
                s = s_3;
                int64_t var_e0_3 = rdx_18;
            }
            else if (rax_16 <= 4)
            {
                if (rax_16 > 1)
                {
                    if (rax_16 - 2 <= 1)
                    {
                        *(arg2 + 1);
                        std::basic_string_view<char>::basic_string_view(&var_c8);
                        uint64_t var_d8_1 = var_c8;
                        std::basic_format_context<std::__format::_Sink_iter<char>, char>* 
                            var_d0_1 = var_c0;
                        int64_t s_1;
                        int64_t rdx_11;
                        s_1 = std::to_chars(i_1, var_f8_1, rsi, var_b8_1);
                        s = s_1;
                        int64_t var_e0_1 = rdx_11;
                    }
                }
                else if (rax_16 >= 0)
                {
                    int64_t s_2;
                    int64_t rdx_14;
                    s_2 = std::to_chars(i_1, var_f8_1, rsi, var_b8_1);
                    s = s_2;
                    int64_t var_e0_2 = rdx_14;
                }
            }
        }
        
        int64_t rax_48;
        
        if (*arg2 & 0x10)
            rax_48 = std::basic_string_view<char>::size();
        
        char rax_49;
        
        if (!(*arg2 & 0x10) || !rax_48)
            rax_49 = 0;
        else
            rax_49 = 1;
        
        if (rax_49)
        {
            i_1 += -(std::basic_string_view<char>::size());
            int64_t rax_52 = std::basic_string_view<char>::size();
            memcpy(i_1, std::basic_string_view<char>::data(), rax_52, i_1);
        }
        
        *arg2;
        std::basic_string_view<char>::basic_string_view(&var_c8, 
            std::__format::__put_sign<int128_t>(rsi, rsi, rdx));
        result =
            std::__format::__formatter_int<char>::_M_format_int<std::__format::_Sink_iter<char> >(
            arg2, var_c8, var_c0);
    }
    else
        result = std::__format::__formatter_int<char>::_M_format_character<std::__format::_Sink_iter<char> >(
            arg2, std::__format::__formatter_int<char>::_S_to_character<int128_t>(rsi));
    *(fsbase + 0x28);
    
    if (rax_2 == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

void* std::__format::__formatter_int<char>::_M_parse<uint128_t>(std::basic_format_parse_context<char>& arg1)
{
    int64_t rsi;
    return std::__format::__formatter_int<char>::_M_do_parse(arg1, rsi);
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_int<char>::format<uint128_t, std::__format::_Sink_iter<char> >(uint128_t arg1, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg2)
{
    void* fsbase;
    int64_t rax_2 = *(fsbase + 0x28);
    int64_t result;
    int64_t rsi;
    
    if ((*(arg2 + 1) & 0x78) != 0x38)
    {
        int64_t s;
        __builtin_memset(&s, 0, 0x20);
        int32_t rax_9 = rsi;
        void var_a5;
        void* i_1 = &var_a5;
        void var_25;
        void* var_f8_1 = &var_25;
        void* i_2 = i_1;
        uint32_t rax_14 = *(arg2 + 1) >> 3 & 0xf;
        uint64_t var_c8;
        std::basic_format_context<std::__format::_Sink_iter<char>, char>* var_c0;
        char* rdx;
        
        if (rax_14 <= 6)
        {
            if (rax_14 >= 5)
            {
                *(arg2 + 1);
                std::basic_string_view<char>::basic_string_view(&var_c8);
                uint64_t var_d8_3 = var_c8;
                std::basic_format_context<std::__format::_Sink_iter<char>, char>* var_d0_3 = var_c0;
                int64_t s_4;
                int64_t rdx_19;
                s_4 = std::to_chars(i_1, var_f8_1, rsi, rax_9);
                s = s_4;
                int64_t var_e0_4 = rdx_19;
                
                if ((*(arg2 + 1) & 0x78) == 0x30)
                {
                    for (void* i = i_1; i != s; i += 1)
                        *i = toupper(*i);
                }
            }
            else if (rax_14 == 4)
            {
                if (rsi | rdx)
                {
                    std::basic_string_view<char>::basic_string_view(&var_c8);
                    uint64_t var_d8_2 = var_c8;
                    std::basic_format_context<std::__format::_Sink_iter<char>, char>* var_d0_2 =
                        var_c0;
                }
                
                int64_t s_3;
                int64_t rdx_15;
                s_3 = std::to_chars(i_1, var_f8_1, rsi, rax_9);
                s = s_3;
                int64_t var_e0_3 = rdx_15;
            }
            else if (rax_14 <= 4)
            {
                if (rax_14 > 1)
                {
                    if (rax_14 - 2 <= 1)
                    {
                        *(arg2 + 1);
                        std::basic_string_view<char>::basic_string_view(&var_c8);
                        uint64_t var_d8_1 = var_c8;
                        std::basic_format_context<std::__format::_Sink_iter<char>, char>* 
                            var_d0_1 = var_c0;
                        int64_t s_1;
                        int64_t rdx_8;
                        s_1 = std::to_chars(i_1, var_f8_1, rsi, rax_9);
                        s = s_1;
                        int64_t var_e0_1 = rdx_8;
                    }
                }
                else if (rax_14 >= 0)
                {
                    int64_t s_2;
                    int64_t rdx_11;
                    s_2 = std::to_chars(i_1, var_f8_1, rsi, rax_9);
                    s = s_2;
                    int64_t var_e0_2 = rdx_11;
                }
            }
        }
        
        int64_t rax_46;
        
        if (*arg2 & 0x10)
            rax_46 = std::basic_string_view<char>::size();
        
        char rax_47;
        
        if (!(*arg2 & 0x10) || !rax_46)
            rax_47 = 0;
        else
            rax_47 = 1;
        
        if (rax_47)
        {
            i_1 += -(std::basic_string_view<char>::size());
            int64_t rax_50 = std::basic_string_view<char>::size();
            memcpy(i_1, std::basic_string_view<char>::data(), rax_50, i_1);
        }
        
        *arg2;
        std::basic_string_view<char>::basic_string_view(&var_c8, 
            std::__format::__put_sign<uint128_t>(rsi, rsi, rdx));
        result =
            std::__format::__formatter_int<char>::_M_format_int<std::__format::_Sink_iter<char> >(
            arg2, var_c8, var_c0);
    }
    else
        result = std::__format::__formatter_int<char>::_M_format_character<std::__format::_Sink_iter<char> >(
            arg2, std::__format::__formatter_int<char>::_S_to_character<uint128_t>(rsi));
    *(fsbase + 0x28);
    
    if (rax_2 == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<_Float128, std::__format::_Sink_iter<char>>(_Float128, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda'(char*, char*)::operator()(char*, char*) const(int64_t* arg1, int64_t arg2, int64_t arg3)
{
    int512_t zmm0;
    
    if (**arg1)
    {
        zmm0 = *arg1[1];
        return std::to_chars(char*, char*, _Float128, std::chars_format, int)(arg2, arg3, *arg1[2], 
            *arg1[3], zmm0);
    }
    
    if (!*arg1[2])
    {
        zmm0 = *arg1[1];
        return std::to_chars(char*, char*, _Float128)(arg2, arg3, zmm0);
    }
    
    zmm0 = *arg1[1];
    return std::to_chars(char*, char*, _Float128, std::chars_format)(arg2, arg3, *arg1[2], zmm0);
}

int64_t std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<_Float128, std::__format::_Sink_iter<char>>(_Float128, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)::operator()(char*, unsigned long) const(int64_t* arg1, int64_t arg2, int64_t arg3)
{
    int64_t* rbx = arg1[1];
    int64_t rax_2;
    int64_t rdx_3;
    rax_2 = std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<_Float128, std::__format::_Sink_iter<char>>(_Float128, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda'(char*, char*)::operator()(char*, char*) const(
        *arg1, arg2 + 1, arg2 + arg3 - 1);
    *rbx = rax_2;
    rbx[1] = rdx_3;
    
    if (*(arg1[1] + 8))
        return 0;
    
    return *arg1[1] - arg2;
}

int64_t std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<_Float128, std::__format::_Sink_iter<char>>(_Float128, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const(std::basic_format_context<std::__format::_Sink_iter<char>, char>* arg1)
{
    int128_t zmm0;
    int128_t var_218 = zmm0;
    void* fsbase;
    int64_t var_20 = *(fsbase + 0x28);
    std::string::string();
    int64_t var_168 = 0;
    int64_t var_160 = 0;
    int64_t var_1e0 = 6;
    
    if (*(arg1 + 1) & 6)
        uint64_t var_1e0_1 = std::__format::_Spec<char>::_M_get_precision<std::basic_format_context<std::__format::_Sink_iter<char>, char> >(
            arg1);
    
    void var_a7;
    void* var_1d0 = &var_a7;
    void var_28;
    void* var_1c8 = &var_28;
    int32_t var_1f0 = 0;
    char var_1f5 = 0;
    char var_1f4 = 0;
    char var_1f3 = 0x65;
    /* jump -> *(((*(arg1 + 1) >> 3 & 0xf) << 2) + &data_41a560) + &data_41a560 */
}

std::ranges::in_out_result<__gnu_cxx::__normal_iterator<char*, std::span<char, 18446744073709551615ul> >, std::__format::_Sink_iter<char> > std::ranges::__copy_fn::operator()<__normal_iterator<char*, std::span<char, 18446744073709551615ul> >, __normal_iterator<char*, std::span<char, 18446744073709551615ul> >, std::__format::_Sink_iter<char> >(__normal_iterator<char*, std::span<char, 18446744073709551615ul> > arg1, __normal_iterator<char*, std::span<char, 18446744073709551615ul> > arg2, std::__format::_Sink_iter<char> arg3)
{
    int64_t var_20 = arg1;
    int64_t var_28 = arg2;
    int64_t var_30 = arg3;
    int64_t rcx;
    int64_t var_38 = rcx;
    std::__format::_Sink_iter<char>** rax = std::move<std::__format::_Sink_iter<char>&>(&var_38);
    __normal_iterator<char*, std::span<char, 18446744073709551615ul> >** rax_1 =
        std::move<__normal_iterator<char*, std::span<char, 18446744073709551615ul> >&>(&var_30);
    return std::ranges::__copy_or_move<false, __normal_iterator<char*, std::span<char, 18446744073709551615ul> >, __normal_iterator<char*, std::span<char, 18446744073709551615ul> >, std::__format::_Sink_iter<char> >(
        *std::move<__normal_iterator<char*, std::span<char, 18446744073709551615ul> >&>(&var_28), 
        *rax_1, *rax);
}

int64_t void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::resize_and_overwrite<std::__format::_Seq_sink<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>::_M_reserve(unsigned long)::'lambda'(auto, auto)&>(unsigned long, auto)::_Terminator::~_Terminator()(int64_t* arg1)
{
    arg1[1];
    return std::string::_M_set_length(*arg1);
}

int64_t auto std::__format::_Seq_sink<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>::_M_reserve(unsigned long)::'lambda'(auto, auto)::operator()<char*, unsigned long>(auto, auto) const(int64_t arg1, int64_t arg2, int64_t arg3) __pure
{
    int64_t var_10 = arg1;
    int64_t var_18 = arg2;
    return arg3;
}

int64_t void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::resize_and_overwrite<std::__format::_Seq_sink<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>::_M_reserve(unsigned long)::'lambda'(auto, auto)&>(unsigned long, auto)(uint64_t arg1, int64_t arg2, int64_t arg3)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    std::string::reserve(arg1);
    int64_t rax_3 = std::string::_M_data();
    
    if (0)
    {
        uint64_t rbx_2 = arg2 - std::string::size();
        std::char_traits<char>::assign(std::string::size() + rax_3, rbx_2, 0);
    }
    
    int128_t var_38 = {0};
    var_38 = arg1;
    *var_38[8] = auto std::__format::_Seq_sink<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>::_M_reserve(unsigned long)::'lambda'(auto, auto)::operator()<char*, unsigned long>(auto, auto) const(
        std::remove_reference<auto>::type&& std::move<std::__format::_Seq_sink<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>::_M_reserve(unsigned long)::'lambda'(auto, auto)&>(auto&&)(
            arg3), 
        rax_3, arg2);
    void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::resize_and_overwrite<std::__format::_Seq_sink<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>::_M_reserve(unsigned long)::'lambda'(auto, auto)&>(unsigned long, auto)::_Terminator::~_Terminator()(
        &var_38);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

char* __normal_iterator<char*, std::span<char, 18446744073709551615ul> >::operator+(int64_t arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t rsi;
    int64_t var_20 = rsi + *arg1;
    char* result;
    __normal_iterator<char*, std::span<char, 18446744073709551615ul> >::__normal_iterator(&result);
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

uint64_t std::string::resize(uint64_t arg1, char arg2)
{
    uint64_t result = arg2;
    char rdx;
    char var_2c = rdx;
    int64_t rax_2 = std::string::size();
    
    if (rax_2 < result)
        return std::string::append(arg1, result - rax_2);
    
    if (result >= rax_2)
        return result;
    
    return std::string::_M_set_length(arg1);
}

char* std::__format::__put_sign<uint32_t>(uint32_t arg1, std::__format::_Sign arg2, char* arg3)
{
    uint32_t var_c = arg1;
    int32_t var_10 = arg2;
    
    if (var_10 != 1)
    {
        if (var_10 != 3)
            return &arg3[1];
        
        *arg3 = 0x20;
    }
    else
        *arg3 = 0x2b;
    
    return arg3;
}

char* std::__format::__put_sign<int32_t>(int32_t arg1, std::__format::_Sign arg2, char* arg3)
{
    int32_t var_10 = arg2;
    
    if (arg1 < 0)
        *arg3 = 0x2d;
    else if (var_10 != 1)
    {
        if (var_10 != 3)
            return &arg3[1];
        
        *arg3 = 0x20;
    }
    else
        *arg3 = 0x2b;
    
    return arg3;
}

char* std::__format::__put_sign<int64_t>(int64_t arg1, std::__format::_Sign arg2, char* arg3)
{
    int32_t var_14 = arg2;
    
    if (arg1 < 0)
        *arg3 = 0x2d;
    else if (var_14 != 1)
    {
        if (var_14 != 3)
            return &arg3[1];
        
        *arg3 = 0x20;
    }
    else
        *arg3 = 0x2b;
    
    return arg3;
}

char* std::__format::__put_sign<uint64_t>(uint64_t arg1, std::__format::_Sign arg2, char* arg3)
{
    uint64_t var_10 = arg1;
    int32_t var_14 = arg2;
    
    if (var_14 != 1)
    {
        if (var_14 != 3)
            return &arg3[1];
        
        *arg3 = 0x20;
    }
    else
        *arg3 = 0x2b;
    
    return arg3;
}

int64_t std::basic_string_view<char>::find_first_not_of(char arg1, uint64_t arg2)
{
    int64_t* var_10 = arg1;
    int64_t result_1;
    int64_t result = result_1;
    char var_14 = arg2;
    
    while (true)
    {
        if (result >= *var_10)
            return -1;
        
        if (std::char_traits<char>::eq(var_10[1] + result, &var_14) != 1)
            break;
        
        result += 1;
    }
    
    return result;
}

uint64_t _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEaSISt17basic_string_viewIcS2_EEENSt9enable_ifIXsrSt6__and_IJSt14is_convertibleIRKT_S7_ESt6__not_ISA_IPSC_PKS4_EESF_ISA_ISD_PKcEEEE5valueERS4_E4typeESD_(char* arg1, int64_t* arg2)
{
    return _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE6assignISt17basic_string_viewIcS2_EEENSt9enable_ifIXsrSt6__and_IJSt14is_convertibleIRKT_S7_ESt6__not_ISA_IPSC_PKS4_EESF_ISA_ISD_PKcEEEE5valueERS4_E4typeESD_(
        arg1, arg2);
}

int64_t std::string::operator+=(char arg1)
{
    int64_t result = arg1;
    std::string::push_back(result);
    return result;
}

uint64_t std::string::append(uint64_t arg1, char arg2)
{
    return std::string::_M_replace_aux(arg1, std::string::size(), 0, arg2);
}

char* _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE6appendISt17basic_string_viewIcS2_EEENSt9enable_ifIXsrSt6__and_IJSt14is_convertibleIRKT_S7_ESt6__not_ISA_IPSC_PKS4_EESF_ISA_ISD_PKcEEEE5valueERS4_E4typeESD_(char* arg1, int64_t* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_38 = *arg2;
    int64_t var_30 = arg2[1];
    std::basic_string_view<char>::size();
    char* result = std::string::append(arg1, std::basic_string_view<char>::data());
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

uint64_t std::string::insert(uint64_t arg1, uint64_t arg2, char arg3)
{
    return std::string::_M_replace_aux(arg1, std::string::_M_check(arg1, arg2), 0, arg3);
}

int64_t std::string::operator[](uint64_t arg1)
{
    int64_t rsi;
    return std::string::_M_data() + rsi;
}

char* std::__format::__put_sign<int128_t>(int128_t arg1, std::__format::_Sign arg2, char* arg3)
{
    int64_t var_18 = arg2;
    char* result;
    int32_t rdx;
    
    if (arg3 < 0)
        *result = 0x2d;
    else if (rdx != 1)
    {
        if (rdx != 3)
            return &result[1];
        
        *result = 0x20;
    }
    else
        *result = 0x2b;
    return result;
}

char* std::__format::__put_sign<uint128_t>(uint128_t arg1, std::__format::_Sign arg2, char* arg3)
{
    int64_t var_18 = arg2;
    char* var_10 = arg3;
    char* result;
    int32_t rdx;
    
    if (rdx != 1)
    {
        if (rdx != 3)
            return &result[1];
        
        *result = 0x20;
    }
    else
        *result = 0x2b;
    
    return result;
}

int64_t std::__format::_Sink<char>::_M_write(std::basic_string_view<char> arg1)
{
    int64_t* var_60 = arg1;
    int64_t rsi;
    int64_t var_78 = rsi;
    int64_t rdx;
    int64_t var_70 = rdx;
    void* fsbase;
    int64_t rax_2 = *(fsbase + 0x28);
    char* var_38 = std::span<char, 18446744073709551615ul>::begin();
    char* rax_10;
    int64_t rdx_4;
    rax_10 = std::span<char, 18446744073709551615ul>::subspan(&var_60[1], 
        operator-<char*, std::span<char, 18446744073709551615ul> >(&var_60[3], &var_38));
    var_38 = rax_10;
    int64_t var_30 = rdx_4;
    
    while (true)
    {
        int64_t rax_28 = std::span<char, 18446744073709551615ul>::size();
        int64_t rax_29;
        rax_29 = std::basic_string_view<char>::size() >= rax_28;
        
        if (!rax_29)
            break;
        
        uint64_t rax_11 = std::span<char, 18446744073709551615ul>::size();
        std::basic_string_view<char>::copy(&var_78, 
            std::span<char, 18446744073709551615ul>::data(), rax_11);
        std::span<char, 18446744073709551615ul>::size();
        __normal_iterator<char*, std::span<char, 18446744073709551615ul> >::operator+=(&var_60[3]);
        int64_t rax_16 = std::span<char, 18446744073709551615ul>::size();
        std::basic_string_view<char>::remove_prefix(&var_78);
        (**var_60)(var_60, rax_16);
        char* var_50 = std::span<char, 18446744073709551615ul>::begin();
        char* rax_27;
        int64_t rdx_11;
        rax_27 = std::span<char, 18446744073709551615ul>::subspan(&var_60[1], 
            operator-<char*, std::span<char, 18446744073709551615ul> >(&var_60[3], &var_50));
        var_38 = rax_27;
        int64_t var_30_1 = rdx_11;
    }
    
    int64_t rax_30;
    rax_30 = std::basic_string_view<char>::size();
    
    if (rax_30)
    {
        uint64_t rax_31 = std::basic_string_view<char>::size();
        std::basic_string_view<char>::copy(&var_78, 
            std::span<char, 18446744073709551615ul>::data(), rax_31);
        std::basic_string_view<char>::size();
        __normal_iterator<char*, std::span<char, 18446744073709551615ul> >::operator+=(&var_60[3]);
    }
    
    if (rax_2 == *(fsbase + 0x28))
        return rax_2 - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

std::locale* std::basic_format_context<std::__format::_Sink_iter<char>, char>::locale()
{
    std::__format::_Optional_locale::value();
    std::locale* result;
    std::locale::locale(result);
    return result;
}

std::string& std::__addressof<std::string>(std::string& arg1) __pure
{
    return arg1;
}

std::string&& std::string::operator=(std::string&& arg1)
{
    char var_51 = 1;
    std::string::_M_is_local();
    int64_t var_38 = std::string::_M_get_allocator();
    std::string* rsi;
    
    if (!std::string::_M_is_local())
    {
        int64_t var_50_1 = 0;
        
        if (std::string::_M_is_local() != 1)
        {
            var_50_1 = std::string::_M_data();
            int64_t var_48 = *(arg1 + 0x10);
        }
        
        std::string::_M_data();
        std::string::_M_data(arg1);
        std::string::length();
        std::string::_M_length(arg1);
        *(rsi + 0x10);
        std::string::_M_capacity(arg1);
        
        if (!var_50_1)
        {
            std::string::_M_local_data();
            std::string::_M_data(rsi);
        }
        else
        {
            std::string::_M_data(rsi);
            std::string::_M_capacity(rsi);
        }
    }
    else
    {
        std::string* rax_17;
        rax_17 = arg1 != std::__addressof<std::string>(rsi);
        uint64_t rax_18;
        rax_18 = rax_17;
        
        if (rax_18)
        {
            int64_t rax_20;
            rax_20 = std::string::size();
            
            if (rax_20)
            {
                uint64_t rax_22 = std::string::size();
                char* rax_24 = std::string::_M_data();
                std::string::_S_copy(std::string::_M_data(), rax_24, rax_22);
            }
            
            std::string::size();
            std::string::_M_set_length(arg1);
        }
    }
    std::string::clear();
    return arg1;
}

uint64_t std::string::operator=(char const* arg1)
{
    return std::string::assign(arg1);
}

uint8_t std::__format::__formatter_int<char>::_S_to_character<uint8_t>(uint8_t arg1)
{
    if (arg1 >= 0)
        return arg1;
    
    std::__throw_format_error("format error: integer not repres…");
    /* no return */
}

char* std::__format::__put_sign<uint8_t>(uint8_t arg1, std::__format::_Sign arg2, char* arg3)
{
    int32_t var_10 = arg2;
    uint8_t var_c = arg1;
    
    if (var_10 != 1)
    {
        if (var_10 != 3)
            return &arg3[1];
        
        *arg3 = 0x20;
    }
    else
        *arg3 = 0x2b;
    
    return arg3;
}

std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_int<char>::_M_format_int<std::__format::_Sink_iter<char> >(std::basic_string_view<char> arg1, uint64_t arg2, std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg3)
{
    char* var_d0 = arg1;
    void* fsbase;
    int64_t rax_2 = *(fsbase + 0x28);
    uint64_t rax_4 = std::__format::_Spec<char>::_M_get_width<std::basic_format_context<std::__format::_Sink_iter<char>, char> >(
        var_d0);
    uint64_t var_78 = 0;
    int64_t var_70 = 0;
    var_78 = arg2;
    std::basic_format_context<std::__format::_Sink_iter<char>, char>& var_70_1 = arg3;
    uint64_t var_68;
    uint64_t rcx;
    
    if (*var_d0 & 0x20)
    {
        std::basic_format_context<std::__format::_Sink_iter<char>, char>::locale();
        void var_c0;
        void* var_b0_1 = &var_c0;
        std::locale::name();
        void var_58;
        char rax_10 = std::operator==<char>(&var_58, u"C…") ^ 1;
        std::string::~string();
        
        if (rax_10)
        {
            std::use_facet<std::numpunct<char> >(var_b0_1);
            std::numpunct<char>::grouping();
            
            if (std::string::empty() != 1)
            {
                int64_t rax_16 = std::basic_string_view<char>::size() - rcx;
                void var_f8;
                uint64_t rax_26 =
                    (&var_f8 - COMBINE(0, rcx + rax_16 * 2 + 0x17) / 0x10 * 0x10 + 0xf) >> 4 << 4;
                char* rax_27 = std::basic_string_view<char>::data();
                std::char_traits<char>::copy(rax_26, rax_27, rcx);
                char* var_90_2 = &rax_27[rcx];
                uint64_t rax_31 = std::string::size();
                char* rax_32 = std::string::data();
                std::__add_grouping<char>(rax_26 + rcx, std::numpunct<char>::thousands_sep(), 
                    rax_32, rax_31, var_90_2, &var_90_2[rax_16]);
                std::basic_string_view<char>::basic_string_view(&var_68, rax_26);
                var_78 = var_68;
                std::basic_format_context<std::__format::_Sink_iter<char>, char>* var_60;
                var_70_1 = var_60;
            }
            
            std::string::~string();
        }
        
        std::locale::~locale();
    }
    
    int64_t rax_40;
    rax_40 = std::basic_string_view<char>::size() >= rax_4;
    int64_t result;
    
    if (!rax_40)
    {
        int32_t var_c8_1 = *(var_d0 + 8);
        uint32_t var_c4_1 = *var_d0 & 3;
        char32_t rdx_20 = rax_4 - std::basic_string_view<char>::size();
        var_68 = std::basic_format_context<std::__format::_Sink_iter<char>, char>::out();
        
        if (!var_c4_1)
        {
            var_c4_1 = 2;
            
            if (!(*var_d0 & 0x40))
                var_c8_1 = 0x20;
            else
            {
                var_c8_1 = 0x30;
                
                if (rcx)
                {
                    int64_t rax_56;
                    int64_t rdx_22;
                    rax_56 = std::basic_string_view<char>::substr(&var_78, 0);
                    var_68 = std::__format::__write<std::__format::_Sink_iter<char>, char>(
                        *std::move<std::__format::_Sink_iter<char>&>(&var_68), rax_56);
                    std::basic_string_view<char>::remove_prefix(&var_78);
                }
            }
        }
        
        result = std::__format::__write_padded<std::__format::_Sink_iter<char>, char>(
            *std::move<std::__format::_Sink_iter<char>&>(&var_68), var_78, var_70_1, var_c4_1, 
            rdx_20);
    }
    else
        result = std::__format::__write<std::__format::_Sink_iter<char>, char>(
            std::basic_format_context<std::__format::_Sink_iter<char>, char>::out(), var_78);
    
    *(fsbase + 0x28);
    
    if (rax_2 == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

void sub_41b65a(void* arg1 @ rbp) __noreturn
{
    std::string::~string();
    std::locale::~locale();
    void* fsbase;
    
    if (*(arg1 - 0x28) != *(fsbase + 0x28))
    {
        __stack_chk_fail();
        /* no return */
    }
    
    _Unwind_Resume();
    __stack_chk_fail();
    /* no return */
}

void sub_41b67c(void* arg1 @ rbp) __noreturn
{
    std::locale::~locale();
    void* fsbase;
    
    if (*(arg1 - 0x28) != *(fsbase + 0x28))
    {
        __stack_chk_fail();
        /* no return */
    }
    
    _Unwind_Resume();
    __stack_chk_fail();
    /* no return */
}

char std::__format::__formatter_int<char>::_S_to_character<int32_t>(int32_t arg1)
{
    if (arg1 >= 0xffffff80 && arg1 <= 0x7f)
        return arg1;
    
    std::__throw_format_error("format error: integer not repres…");
    /* no return */
}

char std::__format::__formatter_int<char>::_S_to_character<uint32_t>(uint32_t arg1)
{
    if (arg1 <= 0x7f)
        return arg1;
    
    std::__throw_format_error("format error: integer not repres…");
    /* no return */
}

char std::__format::__formatter_int<char>::_S_to_character<int64_t>(int64_t arg1)
{
    if (arg1 >= -0x80 && arg1 <= 0x7f)
        return arg1;
    
    std::__throw_format_error("format error: integer not repres…");
    /* no return */
}

char std::__format::__formatter_int<char>::_S_to_character<uint64_t>(uint64_t arg1)
{
    if (arg1 <= 0x7f)
        return arg1;
    
    std::__throw_format_error("format error: integer not repres…");
    /* no return */
}

uint64_t std::__format::_Spec<char>::_M_get_precision<std::basic_format_context<std::__format::_Sink_iter<char>, char> >(std::basic_format_context<std::__format::_Sink_iter<char>, char>& arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    uint64_t result = -1;
    
    if ((*(arg1 + 1) & 6) == 2)
        result = *(arg1 + 6);
    else if ((*(arg1 + 1) & 6) == 4)
    {
        *(arg1 + 6);
        std::basic_format_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char> > 
            var_38;
        std::basic_format_context<std::__format::_Sink_iter<char>, char>::arg(&var_38);
        result = std::__format::__int_from_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char> >(
            &var_38);
    }
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

void* std::__format::__formatter_fp<char>::_M_localize() const::std::basic_string_view<char, std::char_traits<char> >(char arg1, std::locale const& arg2)
{
    int64_t* var_40 = arg1;
    int64_t rdx;
    int64_t var_50 = rdx;
    var_40[2];
    char* r14 = std::basic_string_view<char>::data() + *var_40[3];
    var_40[2];
    char* rax_5 = std::basic_string_view<char>::data();
    var_40[1];
    uint64_t rax_8 = std::string::size();
    var_40[1];
    char* rax_11 = std::string::data();
    *var_40;
    char* var_30 = std::__add_grouping<char>(arg2, std::numpunct<char>::thousands_sep(), rax_11, 
        rax_8, rax_5, r14);
    
    if (*var_40[4])
    {
        if (*var_40[5] != -1)
        {
            *var_30 = *var_40[6];
            var_30 = &var_30[1];
            int64_t* rax_27 = var_40[3];
            *rax_27 += 1;
        }
        
        if (*var_40[4] > 1)
        {
            *var_40[3];
            var_30 = &var_30[std::basic_string_view<char>::copy(var_40[2], var_30, -1)];
        }
    }
    
    return var_30 - arg2;
}

std::cxx11::string std::__format::__formatter_fp<char>::_M_localize()
{
    int64_t rsi;
    int64_t var_118 = rsi;
    int64_t rdx;
    int64_t var_128 = rdx;
    int64_t rcx;
    int64_t var_120 = rcx;
    void* fsbase;
    int64_t rax_1 = *(fsbase + 0x28);
    std::string::string();
    std::locale::classic();
    uint64_t result;
    std::locale* r9;
    
    if (!std::locale::operator==(r9))
    {
        uint64_t rax_7 = std::use_facet<std::numpunct<char> >(r9);
        char var_fb = std::numpunct<char>::decimal_point();
        std::numpunct<char>::grouping();
        char var_fa_1 = 0x2e;
        char rax_13;
        
        if (!std::string::empty() || var_fa_1 != var_fb)
            rax_13 = 0;
        else
            rax_13 = 1;
        
        if (!rax_13)
        {
            uint64_t var_f8 = std::basic_string_view<char>::find(&var_128, var_fa_1);
            char r8;
            uint64_t var_88 = std::basic_string_view<char>::find(&var_128, r8);
            uint64_t var_f0 = *std::min<uint64_t>(&var_f8, &var_88);
            
            if (var_f0 == -1)
                var_f0 = std::basic_string_view<char>::size();
            
            int64_t var_e8 = std::basic_string_view<char>::size() - var_f0;
            void var_48;
            void* var_c0_1 = &var_48;
            int64_t* var_b8_1 = &var_128;
            uint64_t* var_b0_1 = &var_f0;
            int64_t* var_a8_1 = &var_e8;
            uint64_t* var_a0_1 = &var_f8;
            char* var_98_1 = &var_fb;
            std::__format::__formatter_fp<char>::_M_localize(std::basic_string_view<char>, char, std::locale const&) const::'lambda'(char*, uint64_t)
                * rdx_8 = var_f0 * 2 + var_e8;
            var_88 = rax_7;
            void* var_80_1 = var_c0_1;
            int64_t* var_78_1 = var_b8_1;
            uint64_t* var_70_1 = var_b0_1;
            int64_t* var_68_1 = var_a8_1;
            uint64_t* var_60_1 = var_a0_1;
            char* var_58_1 = var_98_1;
            std::string::resize_and_overwrite<std::__format::__formatter_fp<char>::_M_localize(std::basic_string_view<char>, char, std::locale const&) const::'lambda'(char*, uint64_t)&>(
                result, rdx_8);
        }
        
        std::string::~string();
    }
    
    *(fsbase + 0x28);
    
    if (rax_1 == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

int64_t sub_41bc20(void* arg1 @ rbp)
{
    std::string::~string();
    *(arg1 - 0x108);
    std::string::~string();
    void* fsbase;
    
    if (*(arg1 - 0x18) != *(fsbase + 0x28))
    {
        __stack_chk_fail();
        /* no return */
    }
    
    _Unwind_Resume();
    *(fsbase + 0x28);
    
    if (*(arg1 - 0x18) != *(fsbase + 0x28))
    {
        __stack_chk_fail();
        /* no return */
    }
    
    *(arg1 - 8);
    *arg1;
    return *(arg1 - 0x108);
}

int64_t sub_41bc31(void* arg1 @ rbp)
{
    *(arg1 - 0x108);
    std::string::~string();
    void* fsbase;
    
    if (*(arg1 - 0x18) != *(fsbase + 0x28))
    {
        __stack_chk_fail();
        /* no return */
    }
    
    _Unwind_Resume();
    *(fsbase + 0x28);
    
    if (*(arg1 - 0x18) != *(fsbase + 0x28))
    {
        __stack_chk_fail();
        /* no return */
    }
    
    *(arg1 - 8);
    *arg1;
    return *(arg1 - 0x108);
}

auto std::ranges::__access::_Begin::operator()<std::basic_string_view<char>&>(std::basic_string_view<char>&&& arg1)
{
    std::basic_string_view<char>&&& var_10 = arg1;
    return std::basic_string_view<char>::begin();
}

auto std::ranges::__access::_End::operator()<std::basic_string_view<char>&>(std::basic_string_view<char>&&& arg1)
{
    std::basic_string_view<char>&&& var_10 = arg1;
    return std::basic_string_view<char>::end();
}

auto std::__unicode::_Utf_view<char32_t, std::basic_string_view<char> >::_M_begin<char const*, char const*>(char const* arg1, char const* arg2)
{
    char const* var_18 = arg2;
    char* rdx;
    std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_Utf_iterator(
        arg1, rdx, rdx);
    return arg1;
}

char* std::__unicode::_Utf_view<char32_t, std::basic_string_view<char> >::begin()
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    std::ranges::__access::_End::operator()<std::basic_string_view<char>&>(&std::ranges::_Cpo::end);
    std::ranges::__access::_Begin::operator()<std::basic_string_view<char>&>(
        &std::ranges::_Cpo::begin);
    char* rsi;
    char* result;
    std::__unicode::_Utf_view<char32_t, std::basic_string_view<char> >::_M_begin<char const*, char const*>(
        result, rsi);
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

int64_t* std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::begin()
{
    int64_t* rsi;
    int64_t rbx = rsi[1];
    int64_t* result;
    *result = *rsi;
    result[1] = rbx;
    int64_t rbx_1 = rsi[3];
    result[2] = rsi[2];
    result[3] = rbx_1;
    int64_t rbx_2 = rsi[5];
    result[4] = rsi[4];
    result[5] = rbx_2;
    result[6] = rsi[6];
    return result;
}

int64_t std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::_Iterator::end()
{
    return std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::end();
}

int64_t std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::end()
{
    return std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::_Iterator::end();
}

uint64_t std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::_Iterator::width()
{
    int32_t* rdi;
    return std::__unicode::__v15_1_0::__field_width(*rdi);
}

int64_t std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::_Iterator::base()
{
    return std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::base();
}

int64_t std::__unicode::__truncate<char>(std::basic_string_view<char>& arg1, uint64_t arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t result;
    
    if (!std::basic_string_view<char>::empty())
    {
        *arg1;
        *(arg1 + 8);
        void var_98;
        std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::_Grapheme_cluster_view(
            &var_98);
        std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::begin();
        int64_t var_c0_1 =
            std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::end();
        int64_t result_1 = std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::_Iterator::width();
        int64_t var_a8;
        int64_t var_a0;
        
        if (arg2 >= result_1)
        {
            while (true)
            {
                if (std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::_Iterator::operator==(std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::_Iterator::operator++())
                    == 1)
                {
                    result = result_1;
                    break;
                }
                
                int64_t result_2 = result_1 + std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::_Iterator::width();
                
                if (arg2 < result_2)
                {
                    std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::_Iterator::base();
                    std::basic_string_view<char>::basic_string_view<char const*, char const*>(
                        &var_a8, std::basic_string_view<char>::begin());
                    *arg1 = var_a8;
                    *(arg1 + 8) = var_a0;
                    result = result_1;
                    break;
                }
                
                result_1 = result_2;
            }
        }
        else
        {
            std::basic_string_view<char>::basic_string_view();
            *arg1 = var_a8;
            *(arg1 + 8) = var_a0;
            result = 0;
        }
    }
    else
        result = 0;
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::__unicode::__field_width<char>(std::basic_string_view<char> arg1)
{
    int64_t rsi;
    int64_t var_a0 = rsi;
    void* fsbase;
    int64_t rax_2 = *(fsbase + 0x28);
    int64_t result;
    
    if (!std::basic_string_view<char>::empty())
    {
        void var_88;
        std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::_Grapheme_cluster_view(
            &var_88);
        std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::begin();
        int64_t var_98_1 =
            std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::end();
        int64_t result_1 = std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::_Iterator::width();
        
        while (std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::_Iterator::operator==(std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::_Iterator::operator++())
                != 1)
            result_1 += std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::_Iterator::width();
        
        result = result_1;
    }
    else
        result = 0;
    
    *(fsbase + 0x28);
    
    if (rax_2 == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

decltype(auto) std::basic_format_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char> >::_M_visit<std::__format::_WidthPrecVisitor>(std::__format::_WidthPrecVisitor&& arg1, std::__format::_Arg_t arg2)
{
    std::__format::_WidthPrecVisitor&& var_20 = arg1;
    int64_t var_28 = arg2;
    char rdx;
    /* jump -> *((rdx << 2) + &data_41c400) + &data_41c400 */
}

decltype(auto) std::visit_format_arg<std::__format::_WidthPrecVisitor, std::basic_format_context<std::__format::_Sink_iter<char>, char> >(std::__format::_WidthPrecVisitor&& arg1, std::basic_format_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char> > arg2)
{
    std::__format::_WidthPrecVisitor arg_8;
    return std::basic_format_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char> >::_M_visit<std::__format::_WidthPrecVisitor>(
        &arg_8, std::forward<std::__format::_WidthPrecVisitor>(arg1));
}

uint64_t std::__format::__int_from_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char> >(std::basic_format_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char> > const& arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t rdx = *(arg1 + 8);
    int64_t var_48 = *arg1;
    int64_t var_40 = rdx;
    int64_t var_38 = *(arg1 + 0x10);
    int64_t var_30 = *(arg1 + 0x18);
    void var_11;
    int64_t result = std::visit_format_arg<std::__format::_WidthPrecVisitor, std::basic_format_context<std::__format::_Sink_iter<char>, char> >(
        &var_11, &var_48);
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

bool std::__unicode::__is_single_code_unit<char>(char32_t arg1) __pure
{
    return arg1 <= 0x7e;
}

int64_t std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::_M_curr() __pure
{
    int64_t rdi;
    return rdi + 0x10;
}

int32_t* std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::_Utf_iterator(char32_t const* arg1, char32_t const* arg2, char32_t const* arg3)
{
    std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::_First_and_curr<char32_t const*>::_First_and_curr(
        &arg1[2], arg2);
    arg1[6] = 0;
    *(arg1 + 0x19) = 0;
    *(arg1 + 0x1a) = 0;
    int64_t rcx;
    *(arg1 + 0x20) = rcx;
    int64_t rax_9;
    rax_9 = *std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::_M_curr()
        != *(arg1 + 0x20);
    
    if (rax_9)
        return std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::_M_read();
    
    int32_t* result = arg1;
    *result = 0;
    return result;
}

int64_t std::string::string<std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>, void>(std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl> arg1, std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl> arg2, std::allocator<char> const& arg3)
{
    char* var_20 = arg1;
    char* rax_1 = std::string::_M_local_data();
    std::string::_Alloc_hider::_Alloc_hider(var_20, rax_1);
    *(var_20 + 8) = 0;
    int64_t arg_30;
    int64_t var_50 = arg_30;
    int64_t arg_38;
    int64_t var_48 = arg_38;
    int64_t arg_40;
    int64_t var_40 = arg_40;
    int64_t arg_48;
    int64_t var_38 = arg_48;
    int64_t arg_50;
    int64_t var_30 = arg_50;
    int64_t arg_8;
    int64_t var_78 = arg_8;
    int64_t arg_10;
    int64_t var_70 = arg_10;
    int64_t arg_18;
    int64_t var_68 = arg_18;
    int64_t arg_20;
    int64_t var_60 = arg_20;
    int64_t arg_28;
    int64_t var_58 = arg_28;
    return std::string::_M_construct<std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl> >(
        var_20, rax_1, var_20);
}

char std::__format::__formatter_int<char>::_S_to_character<int128_t>(int128_t arg1)
{
    bool o = /* bool o = unimplemented  {sbb rax, 0xffffffffffffffff} */;
    int64_t rsi;
    int64_t rdi;
    
    if (rsi + 1 < 0 == o)
    {
        bool o_1 = /* bool o_1 = unimplemented  {sbb rax, qword [rbp-0x8]} */;
        
        if (0 - rsi < 0 == o_1)
            return rdi;
    }
    
    std::__throw_format_error("format error: integer not repres…");
    /* no return */
}

char std::__format::__formatter_int<char>::_S_to_character<uint128_t>(uint128_t arg1)
{
    bool c_1 = /* bool c_1 = unimplemented  {sbb rax, qword [rbp-0x8]} */;
    
    if (!c_1)
    {
        int64_t rdi;
        return rdi;
    }
    
    std::__throw_format_error("format error: integer not repres…");
    /* no return */
}

__normal_iterator<char*, std::span<char, 18446744073709551615ul> >&&& std::move<__normal_iterator<char*, std::span<char, 18446744073709551615ul> >&>(__normal_iterator<char*, std::span<char, 18446744073709551615ul> >&&& arg1) __pure
{
    return arg1;
}

std::__conditional<false>::type<std::ranges::in_out_result<__gnu_cxx::__normal_iterator<char*, std::span<char, 18446744073709551615ul> >, std::__format::_Sink_iter<char> >, std::ranges::in_out_result<__gnu_cxx::__normal_iterator<char*, std::span<char, 18446744073709551615ul> >, std::__format::_Sink_iter<char> > > std::ranges::__copy_or_move<false, __normal_iterator<char*, std::span<char, 18446744073709551615ul> >, __normal_iterator<char*, std::span<char, 18446744073709551615ul> >, std::__format::_Sink_iter<char> >(__normal_iterator<char*, std::span<char, 18446744073709551615ul> > arg1, __normal_iterator<char*, std::span<char, 18446744073709551615ul> > arg2, std::__format::_Sink_iter<char> arg3)
{
    int64_t var_50 = arg1;
    int64_t var_58 = arg2;
    int64_t var_60 = arg3;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    std::__format::_Sink_iter<char>** rax_1 = std::move<std::__format::_Sink_iter<char>&>(&var_60);
    char* rbx = *__normal_iterator<char*, std::span<char, 18446744073709551615ul> >::base();
    int64_t rax_5;
    int64_t rdx_1;
    rax_5 = std::ranges::__copy_or_move<false, char*, char*, std::__format::_Sink_iter<char> >(
        *__normal_iterator<char*, std::span<char, 18446744073709551615ul> >::base(), rbx, *rax_1);
    int64_t var_48 = rax_5;
    int64_t var_40 = rdx_1;
    char* result;
    __normal_iterator<char*, std::span<char, 18446744073709551615ul> >::__normal_iterator(&result);
    std::__format::_Sink_iter<char>* var_30 = *std::move<std::__format::_Sink_iter<char>&>(&var_40);
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::remove_reference<auto>::type&& std::move<std::__format::_Seq_sink<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>>::_M_reserve(unsigned long)::'lambda'(auto, auto)&>(auto&&)(int64_t arg1) __pure
{
    return arg1;
}

bool std::operator==<char>(std::string const& arg1, char const* arg2)
{
    if (std::string::size() == std::char_traits<char>::length(arg2))
    {
        uint64_t rax_5 = std::string::size();
        
        if (!std::char_traits<char>::compare(std::string::data(), arg2, rax_5))
            return 1;
    }
    
    return 0;
}

uint64_t _ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE6assignISt17basic_string_viewIcS2_EEENSt9enable_ifIXsrSt6__and_IJSt14is_convertibleIRKT_S7_ESt6__not_ISA_IPSC_PKS4_EESF_ISA_ISD_PKcEEEE5valueERS4_E4typeESD_(char* arg1, int64_t* arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t var_38 = *arg2;
    int64_t var_30 = arg2[1];
    std::basic_string_view<char>::size();
    uint64_t result = std::string::assign(arg1, std::basic_string_view<char>::data());
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::string::push_back(char arg1)
{
    uint64_t var_30 = arg1;
    char rsi;
    char var_34 = rsi;
    uint64_t rax_2 = std::string::size();
    int64_t rax_5;
    rax_5 = std::string::capacity() < rax_2 + 1;
    
    if (rax_5)
        std::string::_M_mutate(var_30, rax_2, nullptr, 0);
    
    std::char_traits<char>::assign(rax_2 + std::string::_M_data(), &var_34);
    return std::string::_M_set_length(var_30);
}

uint64_t std::string::_M_replace_aux(uint64_t arg1, uint64_t arg2, uint64_t arg3, char arg4)
{
    uint64_t var_58 = arg4;
    std::string::_M_check_length(arg1, arg3, var_58);
    int64_t rax_3 = std::string::size();
    int64_t rax_8;
    rax_8 = std::string::capacity() >= var_58 + rax_3 - arg3;
    
    if (!rax_8)
        std::string::_M_mutate(arg1, arg2, arg3, 0);
    else
    {
        void* rax_11 = std::string::_M_data() + arg2;
        uint64_t rax_14 = rax_3 - arg2 - arg3;
        
        if (rax_14 && arg3 != var_58)
            std::string::_S_move(rax_11 + var_58, rax_11 + arg3, rax_14);
    }
    
    char r8;
    
    if (var_58)
        std::string::_S_assign(std::string::_M_data() + arg2, var_58, r8);
    std::string::_M_set_length(arg1);
    return arg1;
}

std::remove_reference<std::__format::_WidthPrecVisitor>::type& std::forward<std::__format::_WidthPrecVisitor>(std::remove_reference<std::__format::_WidthPrecVisitor>::type& arg1) __pure
{
    return arg1;
}

void std::__format::_WidthPrecVisitor::operator()<std::monostate>(std::monostate& arg1) __noreturn
{
    std::monostate& var_10 = arg1;
    int64_t rsi;
    int64_t var_18 = rsi;
    std::__format::__invalid_arg_id_in_format_string();
    /* no return */
}

void std::__format::_WidthPrecVisitor::operator()<bool>(bool& arg1) __noreturn
{
    bool& var_10 = arg1;
    int64_t rsi;
    int64_t var_18 = rsi;
    std::__throw_format_error("format error: argument used for …");
    /* no return */
}

int64_t std::__format::_WidthPrecVisitor::operator()<int32_t>(int32_t& arg1)
{
    int32_t& var_10 = arg1;
    int32_t* rsi;
    
    if (*rsi >= 0)
        return *rsi;
    
    std::__throw_format_error("format error: argument used for …");
    /* no return */
}

uint64_t std::__format::_WidthPrecVisitor::operator()<uint32_t>(uint32_t& arg1)
{
    uint32_t& var_10 = arg1;
    int32_t* rsi;
    return *rsi;
}

uint64_t std::__format::_WidthPrecVisitor::operator()<int64_t>(int64_t& arg1)
{
    int64_t& var_10 = arg1;
    int64_t* rsi;
    
    if (*rsi >= 0)
        return *rsi;
    
    std::__throw_format_error("format error: argument used for …");
    /* no return */
}

uint64_t std::__format::_WidthPrecVisitor::operator()<uint64_t>(uint64_t& arg1)
{
    uint64_t& var_10 = arg1;
    int64_t* rsi;
    return *rsi;
}

void std::__format::_WidthPrecVisitor::operator()<float>(float& arg1) __noreturn
{
    float& var_10 = arg1;
    int64_t rsi;
    int64_t var_18 = rsi;
    std::__throw_format_error("format error: argument used for …");
    /* no return */
}

void std::__format::_WidthPrecVisitor::operator()<double>(double& arg1) __noreturn
{
    double& var_10 = arg1;
    int64_t rsi;
    int64_t var_18 = rsi;
    std::__throw_format_error("format error: argument used for …");
    /* no return */
}

void std::__format::_WidthPrecVisitor::operator()<long double>(long double& arg1) __noreturn
{
    long double& var_10 = arg1;
    int64_t rsi;
    int64_t var_18 = rsi;
    std::__throw_format_error("format error: argument used for …");
    /* no return */
}

void std::__format::_WidthPrecVisitor::operator()<void const*>(void const*& arg1) __noreturn
{
    void const*& var_10 = arg1;
    int64_t rsi;
    int64_t var_18 = rsi;
    std::__throw_format_error("format error: argument used for …");
    /* no return */
}

void std::__format::_WidthPrecVisitor::operator()<int128_t>(int128_t& arg1) __noreturn
{
    int128_t& var_10 = arg1;
    int64_t rsi;
    int64_t var_18 = rsi;
    std::__throw_format_error("format error: argument used for …");
    /* no return */
}

void std::__format::_WidthPrecVisitor::operator()<uint128_t>(uint128_t& arg1) __noreturn
{
    uint128_t& var_10 = arg1;
    int64_t rsi;
    int64_t var_18 = rsi;
    std::__throw_format_error("format error: argument used for …");
    /* no return */
}

void std::__format::_WidthPrecVisitor::operator()<_Float128>(_Float128&) const() __noreturn
{
    int64_t rdi;
    int64_t var_10 = rdi;
    int64_t rsi;
    int64_t var_18 = rsi;
    std::__throw_format_error("format error: argument used for …");
    /* no return */
}

uint64_t std::basic_string_view<char>::copy(char* arg1, uint64_t arg2, uint64_t arg3)
{
    uint64_t var_40 = arg3;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    uint64_t rcx;
    uint64_t rax_4 =
        std::__sv_check(std::basic_string_view<char>::size(), rcx, "basic_string_view::copy");
    uint64_t var_20 = *arg1 - rax_4;
    uint64_t result = *std::min<uint64_t>(&var_40, &var_20);
    std::char_traits<char>::copy(arg2, std::basic_string_view<char>::data() + rax_4, result);
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

std::string const& std::__addressof<std::string const>(std::string const& arg1) __pure
{
    return arg1;
}

int64_t std::string::_M_assign(std::string const& arg1)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    std::string* rax_2;
    std::string* rsi;
    rax_2 = arg1 != std::__addressof<std::string const>(rsi);
    
    if (rax_2)
    {
        uint64_t rax_4 = std::string::length();
        
        if (std::string::capacity() < rax_4)
        {
            uint64_t var_40 = rax_4;
            std::string::_M_create(arg1, &var_40);
            std::string::_M_dispose();
            std::string::_M_data(arg1);
            std::string::_M_capacity(arg1);
        }
        
        if (rax_4)
        {
            char* rax_15 = std::string::_M_data();
            std::string::_S_copy(std::string::_M_data(), rax_15, rax_4);
        }
        
        std::string::_M_set_length(arg1);
    }
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::string::clear()
{
    uint64_t rdi;
    return std::string::_M_set_length(rdi);
}

uint64_t std::string::assign(char const* arg1)
{
    uint64_t rsi;
    std::char_traits<char>::length(rsi);
    return std::string::_M_replace(arg1, 0, std::string::size(), rsi);
}

char* std::__add_grouping<char>(char* arg1, char arg2, char const* arg3, uint64_t arg4, char const* arg5, char const* arg6)
{
    char* result = arg1;
    char const* var_50 = arg5;
    char const* var_58 = arg6;
    void* var_18 = nullptr;
    int64_t var_10 = 0;
    
    while (var_58 - var_50 > *(var_18 + arg3))
    {
        if (*(var_18 + arg3) <= 0)
            break;
        
        if (*(var_18 + arg3) == 0x7f)
            break;
        
        var_58 = &var_58[-(*(var_18 + arg3))];
        
        if (var_18 >= arg4 - 1)
            var_10 += 1;
        else
            var_18 += 1;
    }
    
    while (var_50 != var_58)
    {
        char const* rax_20 = var_50;
        var_50 = &rax_20[1];
        char* result_1 = result;
        result = &result_1[1];
        *result_1 = *rax_20;
    }
    
    while (true)
    {
        int64_t rax_28 = var_10;
        var_10 = rax_28 - 1;
        rax_28 = rax_28;
        
        if (!rax_28)
            break;
        
        char* result_2 = result;
        result = &result_2[1];
        *result_2 = arg2;
        
        for (char i = *(var_18 + arg3); i > 0; i -= 1)
        {
            char const* rax_25 = var_50;
            var_50 = &rax_25[1];
            char* result_3 = result;
            result = &result_3[1];
            *result_3 = *rax_25;
        }
    }
    
    while (true)
    {
        void* rax_35 = var_18;
        var_18 = rax_35 - 1;
        rax_35 = rax_35;
        
        if (!rax_35)
            break;
        
        char* result_4 = result;
        result = &result_4[1];
        *result_4 = arg2;
        
        for (char i_1 = *(var_18 + arg3); i_1 > 0; i_1 -= 1)
        {
            char const* rax_32 = var_50;
            var_50 = &rax_32[1];
            char* result_5 = result;
            result = &result_5[1];
            *result_5 = *rax_32;
        }
    }
    
    return result;
}

int64_t void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::resize_and_overwrite<std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<float, std::__format::_Sink_iter<char>>(float, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)&>(unsigned long, float)::_Terminator::~_Terminator()(int64_t* arg1)
{
    arg1[1];
    return std::string::_M_set_length(*arg1);
}

int64_t void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::resize_and_overwrite<std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<float, std::__format::_Sink_iter<char>>(float, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)&>(unsigned long, float)(uint64_t arg1, int64_t arg2, int64_t arg3)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    std::string::reserve(arg1);
    void* rax_3 = std::string::_M_data();
    
    if (0)
    {
        uint64_t rbx_2 = arg2 - std::string::size();
        std::char_traits<char>::assign(std::string::size() + rax_3, rbx_2, 0);
    }
    
    int128_t var_38 = {0};
    var_38 = arg1;
    *var_38[8] = std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<float, std::__format::_Sink_iter<char>>(float, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)::operator()(char*, unsigned long) const(
        std::remove_reference<float>::type&& std::move<std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<float, std::__format::_Sink_iter<char>>(float, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)&>(float&&)(
            arg3), 
        rax_3, arg2);
    void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::resize_and_overwrite<std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<float, std::__format::_Sink_iter<char>>(float, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)&>(unsigned long, float)::_Terminator::~_Terminator()(
        &var_38);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::__format::_Sink<char>::_M_write(char arg1)
{
    int64_t* var_40 = arg1;
    void* fsbase;
    int64_t rax_1 = *(fsbase + 0x28);
    char* var_30 = __normal_iterator<char*, std::span<char, 18446744073709551615ul> >::operator++(
        var_40 + 0x18);
    char rsi;
    *__normal_iterator<char*, std::span<char, 18446744073709551615ul> >::operator*() = rsi;
    var_30 = std::span<char, 18446744073709551615ul>::begin();
    int64_t rax_14;
    rax_14 = operator-<char*, std::span<char, 18446744073709551615ul> >(&var_40[3], &var_30)
        == std::span<char, 18446744073709551615ul>::size();
    
    if (rax_14)
        (**var_40)(var_40);
    
    if (rax_1 == *(fsbase + 0x28))
        return rax_1 - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::resize_and_overwrite<std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<double, std::__format::_Sink_iter<char>>(double, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)&>(unsigned long, double)::_Terminator::~_Terminator()(int64_t* arg1)
{
    arg1[1];
    return std::string::_M_set_length(*arg1);
}

int64_t void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::resize_and_overwrite<std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<double, std::__format::_Sink_iter<char>>(double, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)&>(unsigned long, double)(uint64_t arg1, int64_t arg2, int64_t arg3)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    std::string::reserve(arg1);
    void* rax_3 = std::string::_M_data();
    
    if (0)
    {
        uint64_t rbx_2 = arg2 - std::string::size();
        std::char_traits<char>::assign(std::string::size() + rax_3, rbx_2, 0);
    }
    
    int128_t var_38 = {0};
    var_38 = arg1;
    *var_38[8] = std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<double, std::__format::_Sink_iter<char>>(double, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)::operator()(char*, unsigned long) const(
        std::remove_reference<double>::type&& std::move<std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<double, std::__format::_Sink_iter<char>>(double, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)&>(double&&)(
            arg3), 
        rax_3, arg2);
    void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::resize_and_overwrite<std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<double, std::__format::_Sink_iter<char>>(double, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)&>(unsigned long, double)::_Terminator::~_Terminator()(
        &var_38);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::resize_and_overwrite<std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<long double, std::__format::_Sink_iter<char>>(long double, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)&>(unsigned long, long double)::_Terminator::~_Terminator()(int64_t* arg1)
{
    arg1[1];
    return std::string::_M_set_length(*arg1);
}

int64_t void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::resize_and_overwrite<std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<long double, std::__format::_Sink_iter<char>>(long double, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)&>(unsigned long, long double)(uint64_t arg1, int64_t arg2, int64_t arg3)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    std::string::reserve(arg1);
    void* rax_3 = std::string::_M_data();
    
    if (0)
    {
        uint64_t rbx_2 = arg2 - std::string::size();
        std::char_traits<char>::assign(std::string::size() + rax_3, rbx_2, 0);
    }
    
    int128_t var_38 = {0};
    var_38 = arg1;
    *var_38[8] = std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<long double, std::__format::_Sink_iter<char>>(long double, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)::operator()(char*, unsigned long) const(
        std::remove_reference<long double>::type&& std::move<std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<long double, std::__format::_Sink_iter<char>>(long double, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)&>(long double&&)(
            arg3), 
        rax_3, arg2);
    void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::resize_and_overwrite<std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<long double, std::__format::_Sink_iter<char>>(long double, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)&>(unsigned long, long double)::_Terminator::~_Terminator()(
        &var_38);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

std::basic_string_view<char>&&& std::move<std::basic_string_view<char>&>(std::basic_string_view<char>&&& arg1) __pure
{
    return arg1;
}

int64_t std::__unicode::_Utf_view<char32_t, std::basic_string_view<char> >::_Utf_view(std::basic_string_view<char>&& arg1)
{
    std::remove_reference<std::basic_string_view<char> >::type* rsi;
    std::remove_reference<std::basic_string_view<char> >::type* rax_1 =
        std::forward<std::basic_string_view<char> >(rsi);
    int64_t rdx = *(rax_1 + 8);
    int64_t result = *rax_1;
    *arg1 = result;
    *(arg1 + 8) = rdx;
    return result;
}

std::remove_reference<std::basic_string_view<char> >::type& std::forward<std::basic_string_view<char> >(std::remove_reference<std::basic_string_view<char> >::type& arg1) __pure
{
    return arg1;
}

int64_t std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::_Grapheme_cluster_view(std::basic_string_view<char> arg1)
{
    int64_t var_60 = arg1;
    int64_t rsi;
    int64_t var_78 = rsi;
    int64_t rdx;
    int64_t var_70 = rdx;
    void* fsbase;
    int64_t rax_2 = *(fsbase + 0x28);
    void var_58;
    std::__unicode::_Utf_view<char32_t, std::basic_string_view<char> >::_Utf_view(&var_58);
    std::__unicode::_Utf_view<char32_t, std::basic_string_view<char> >::begin();
    int64_t var_48;
    int64_t var_a8 = var_48;
    int64_t var_40;
    int64_t var_a0 = var_40;
    int64_t var_38;
    int64_t var_98 = var_38;
    int64_t var_30;
    int64_t var_90 = var_30;
    int64_t var_28;
    int64_t var_88 = var_28;
    std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::_Iterator::_Iterator(
        var_60);
    
    if (rax_2 == *(fsbase + 0x28))
        return rax_2 - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::end()
{
    void* rdi;
    return *(rdi + 0x20);
}

void* std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::_Iterator::operator++()
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    char* rax_3 = std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::end();
    void* result;
    int64_t var_a8 = *(result + 0x10);
    int64_t var_a0 = *(result + 0x18);
    int64_t var_98 = *(result + 0x20);
    int64_t var_90 = *(result + 0x28);
    int64_t var_88 = *(result + 0x30);
    
    if (std::__unicode::operator==(rax_3, rax_3) != 1)
    {
        int32_t var_5c_1 = *(result + 4);
        int64_t rcx_2 = *(result + 0x10);
        int64_t rbx_2 = *(result + 0x18);
        int64_t rcx_3 = *(result + 0x20);
        int64_t rbx_3 = *(result + 0x28);
        int64_t rax_11 = *(result + 0x30);
        
        while (true)
        {
            int64_t* rax_21 = std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::operator++();
            int64_t var_a8_2 = *rax_21;
            int64_t var_a0_2 = rax_21[1];
            int64_t var_98_2 = rax_21[2];
            int64_t var_90_2 = rax_21[3];
            int64_t var_88_2 = rax_21[4];
            
            if (std::__unicode::operator==(rax_3, rax_3) == 1)
                break;
            
            int32_t rax_12 = std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::operator*();
            int32_t rax_14 = std::__unicode::__v15_1_0::__grapheme_cluster_break_property(std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::operator*());
            std::__unicode::__v15_1_0::_Grapheme_cluster_iterator_base::_M_update_xpicto_seq_state(
                result, rax_12);
            std::__unicode::__v15_1_0::_Grapheme_cluster_iterator_base::_M_update_ri_count(result);
            std::__unicode::__v15_1_0::_Grapheme_cluster_iterator_base::_M_update_incb_state(
                result, rax_12);
            int64_t var_a8_1 = rcx_2;
            int64_t var_a0_1 = rbx_2;
            int64_t var_98_1 = rcx_3;
            int64_t var_90_1 = rbx_3;
            int64_t var_88_1 = rax_11;
            
            if (std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::_Iterator::_M_is_break(
                result, var_5c_1, rax_14))
            {
                std::__unicode::__v15_1_0::_Grapheme_cluster_iterator_base::_M_reset(result, 
                    rax_12);
                break;
            }
            
            var_5c_1 = rax_14;
        }
        
        *(result + 0x10) = rcx_2;
        *(result + 0x18) = rbx_2;
        *(result + 0x20) = rcx_3;
        *(result + 0x28) = rbx_3;
        *(result + 0x30) = rax_11;
    }
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::_Iterator::operator==(char const* const& arg1)
{
    int64_t* rsi;
    char* rsi_1 = *rsi;
    int64_t var_58 = arg1[2];
    int64_t var_50 = arg1[3];
    int64_t var_48 = arg1[4];
    int64_t var_40 = arg1[5];
    int64_t var_38 = arg1[6];
    return std::__unicode::operator==(rsi_1, rsi_1);
}

void std::__format::_WidthPrecVisitor::operator()<char>(char& arg1) __noreturn
{
    char& var_10 = arg1;
    int64_t rsi;
    int64_t var_18 = rsi;
    std::__throw_format_error("format error: argument used for …");
    /* no return */
}

void std::__format::_WidthPrecVisitor::operator()<char const*>(char const*& arg1) __noreturn
{
    char const*& var_10 = arg1;
    int64_t rsi;
    int64_t var_18 = rsi;
    std::__throw_format_error("format error: argument used for …");
    /* no return */
}

void std::__format::_WidthPrecVisitor::operator()<std::basic_string_view<char> >(std::basic_string_view<char>& arg1) __noreturn
{
    std::basic_string_view<char>& var_10 = arg1;
    int64_t rsi;
    int64_t var_18 = rsi;
    std::__throw_format_error("format error: argument used for …");
    /* no return */
}

void std::__format::_WidthPrecVisitor::operator()<std::basic_format_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char> >::handle>(std::basic_format_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char> >::handle& arg1) __noreturn
{
    std::basic_format_arg<std::basic_format_context<std::__format::_Sink_iter<char>, char> >::handle
        & var_10 = arg1;
    int64_t rsi;
    int64_t var_18 = rsi;
    std::__throw_format_error("format error: argument used for …");
    /* no return */
}

char32_t const* std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::_First_and_curr<char32_t const*>::_First_and_curr(char32_t const* arg1, char32_t const* arg2)
{
    *arg1 = arg2;
    int64_t rdx;
    *(arg1 + 8) = rdx;
    return arg1;
}

int64_t std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::_M_read()
{
    return std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::_M_read_utf32();
}

int64_t* void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::_M_construct<std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>>(std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>, std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>, std::input_iterator_tag)::_Guard::_Guard(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*)(int64_t* arg1, int64_t arg2)
{
    *arg1 = arg2;
    return arg1;
}

uint64_t void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::_M_construct<std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>>(std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>, std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>, std::input_iterator_tag)::_Guard::~_Guard()(int64_t* arg1)
{
    uint64_t result = *arg1;
    
    if (!result)
        return result;
    
    *arg1;
    return std::string::_M_dispose();
}

int64_t std::string::_M_construct<std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl> >(std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl> arg1, std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl> arg2, std::input_iterator_tag arg3)
{
    uint64_t var_60 = arg1;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    uint64_t var_40 = 0;
    int64_t var_50 = 0xf;
    int64_t arg_8;
    int64_t arg_10;
    int64_t arg_18;
    int64_t arg_20;
    int64_t arg_28;
    int64_t arg_30;
    int64_t arg_38;
    int64_t arg_40;
    int64_t arg_48;
    int64_t arg_50;
    
    while (true)
    {
        int64_t var_90_1 = arg_30;
        int64_t var_88_1 = arg_38;
        int64_t var_80_1 = arg_40;
        int64_t var_78_1 = arg_48;
        int64_t var_70_1 = arg_50;
        int64_t var_b8_1 = arg_8;
        int64_t var_b0_1 = arg_10;
        int64_t var_a8_1 = arg_18;
        int64_t var_a0_1 = arg_20;
        int64_t var_98_1 = arg_28;
        char rax_10;
        
        if (std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::friend operator==(std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>, std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>)(
                arg1, arg2) == 1 || var_40 >= var_50)
            rax_10 = 0;
        else
            rax_10 = 1;
        
        if (!rax_10)
            break;
        
        uint64_t rax_6 = var_40;
        var_40 = rax_6 + 1;
        *(var_60 + rax_6 + 0x10) = std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::operator*();
        arg2 = std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::operator++();
    }
    
    uint64_t rsi = var_60;
    int64_t var_48;
    int64_t* rdi = &var_48;
    void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::_M_construct<std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>>(std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>, std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>, std::input_iterator_tag)::_Guard::_Guard(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>*)(
        rdi, rsi);
    
    while (true)
    {
        int64_t var_90_2 = arg_30;
        int64_t var_88_2 = arg_38;
        int64_t var_80_2 = arg_40;
        int64_t var_78_2 = arg_48;
        int64_t var_70_2 = arg_50;
        int64_t var_b8_2 = arg_8;
        int64_t var_b0_2 = arg_10;
        int64_t var_a8_2 = arg_18;
        int64_t var_a0_2 = arg_20;
        int64_t var_98_2 = arg_28;
        
        if (std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::friend operator==(std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>, std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>)(
                rdi, rsi) == 1)
            break;
        
        if (var_40 == var_50)
        {
            var_50 = var_40 + 1;
            std::string::_S_copy(std::string::_M_create(var_60, &var_50), std::string::_M_data(), 
                var_40);
            std::string::_M_dispose();
            std::string::_M_data(var_60);
            std::string::_M_capacity(var_60);
        }
        
        char var_51 = std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::operator*();
        uint64_t rax_25 = var_40;
        var_40 = rax_25 + 1;
        std::char_traits<char>::assign(std::string::_M_data() + rax_25, &var_51);
        rsi = std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::operator++();
    }
    
    var_48 = 0;
    std::string::_M_set_length(var_60);
    void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::_M_construct<std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>>(std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>, std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>, std::input_iterator_tag)::_Guard::~_Guard()(
        &var_48);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

void sub_41ddb1(void* arg1 @ rbp) __noreturn
{
    void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::_M_construct<std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>>(std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>, std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>, std::input_iterator_tag)::_Guard::~_Guard()(
        arg1 - 0x40);
    void* fsbase;
    
    if (*(arg1 - 0x18) != *(fsbase + 0x28))
    {
        __stack_chk_fail();
        /* no return */
    }
    
    _Unwind_Resume();
    __stack_chk_fail();
    /* no return */
}

int64_t void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::resize_and_overwrite<std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<_Float128, std::__format::_Sink_iter<char>>(_Float128, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)&>(unsigned long, _Float128)::_Terminator::~_Terminator()(int64_t* arg1)
{
    arg1[1];
    return std::string::_M_set_length(*arg1);
}

int64_t void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::resize_and_overwrite<std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<_Float128, std::__format::_Sink_iter<char>>(_Float128, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)&>(unsigned long, _Float128)(uint64_t arg1, int64_t arg2, int64_t arg3)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    std::string::reserve(arg1);
    int64_t rax_3 = std::string::_M_data();
    
    if (0)
    {
        uint64_t rbx_2 = arg2 - std::string::size();
        std::char_traits<char>::assign(std::string::size() + rax_3, rbx_2, 0);
    }
    
    int128_t var_38 = {0};
    var_38 = arg1;
    *var_38[8] = std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<_Float128, std::__format::_Sink_iter<char>>(_Float128, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)::operator()(char*, unsigned long) const(
        std::remove_reference<_Float128>::type&& std::move<std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<_Float128, std::__format::_Sink_iter<char>>(_Float128, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)&>(_Float128&&)(
            arg3), 
        rax_3, arg2);
    void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::resize_and_overwrite<std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<_Float128, std::__format::_Sink_iter<char>>(_Float128, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)&>(unsigned long, _Float128)::_Terminator::~_Terminator()(
        &var_38);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

char*&&& std::move<char*&>(char*&&& arg1) __pure
{
    return arg1;
}

std::__conditional<false>::type<std::ranges::in_out_result<char*, std::__format::_Sink_iter<char> >, std::ranges::in_out_result<char*, std::__format::_Sink_iter<char> > > std::ranges::__copy_or_move<false, char*, char*, std::__format::_Sink_iter<char> >(char* arg1, char* arg2, std::__format::_Sink_iter<char> arg3)
{
    char*& var_40 = arg1;
    int64_t var_50 = arg3;
    
    for (void* i = arg2 - var_40; i > 0; i -= 1)
    {
        *var_40;
        std::__format::_Sink<char>::_M_write(var_50);
        var_40 += 1;
    }
    
    char** result = *std::move<char*&>(&var_40);
    *std::move<std::__format::_Sink_iter<char>&>(&var_50);
    return result;
}

uint64_t std::string::assign(char const* arg1, uint64_t arg2)
{
    return std::string::_M_replace(arg1, 0, std::string::size(), arg2);
}

char* std::string::_S_assign(char* arg1, uint64_t arg2, char arg3)
{
    char var_1c = arg3;
    
    if (arg2 != 1)
        return std::char_traits<char>::assign(arg1, arg2, var_1c);
    
    return std::char_traits<char>::assign(arg1, &var_1c);
}

int64_t std::remove_reference<float>::type&& std::move<std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<float, std::__format::_Sink_iter<char>>(float, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)&>(float&&)(int64_t arg1) __pure
{
    return arg1;
}

int64_t void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::resize_and_overwrite<std::__format::__formatter_fp<char>::_M_localize(std::basic_string_view<char, std::char_traits<char>>, char, std::locale const&) const::'lambda'(char*, unsigned long)&>(unsigned long, std::__format::__formatter_fp<char>::_M_localize(std::basic_string_view<char, std::char_traits<char>>, char, std::locale const&) const::'lambda'(char*, unsigned long)&)::_Terminator::~_Terminator()(int64_t* arg1)
{
    arg1[1];
    return std::string::_M_set_length(*arg1);
}

int64_t std::string::resize_and_overwrite<std::__format::__formatter_fp<char>::_M_localize(std::basic_string_view<char>, char, std::locale const&) const::'lambda'(char*, uint64_t)&>(uint64_t arg1, std::__format::__formatter_fp<char>::_M_localize(std::basic_string_view<char>, char, std::locale const&) const::'lambda'(char*, uint64_t)& arg2)
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    std::string::reserve(arg1);
    std::locale* rax_3 = std::string::_M_data();
    
    if (0)
    {
        void* rbx_2 = arg2 - std::string::size();
        std::char_traits<char>::assign(std::string::size() + rax_3, rbx_2, 0);
    }
    
    int128_t var_38 = {0};
    var_38 = arg1;
    std::__format::__formatter_fp<char>::_M_localize(std::basic_string_view<char>, char, std::locale const&) const::'lambda'(char*, uint64_t)
        ** rdx;
    *var_38[8] = std::__format::__formatter_fp<char>::_M_localize() const::std::basic_string_view<char, std::char_traits<char> >(
        std::move<std::__format::__formatter_fp<char>::_M_localize(std::basic_string_view<char>, char, std::locale const&) const::'lambda'(char*, uint64_t)&>(
            rdx), 
        rax_3);
    void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::resize_and_overwrite<std::__format::__formatter_fp<char>::_M_localize(std::basic_string_view<char, std::char_traits<char>>, char, std::locale const&) const::'lambda'(char*, unsigned long)&>(unsigned long, std::__format::__formatter_fp<char>::_M_localize(std::basic_string_view<char, std::char_traits<char>>, char, std::locale const&) const::'lambda'(char*, unsigned long)&)::_Terminator::~_Terminator()(
        &var_38);
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

void sub_41e1de(void* arg1 @ rbp) __noreturn
{
    void std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char>>::resize_and_overwrite<std::__format::__formatter_fp<char>::_M_localize(std::basic_string_view<char, std::char_traits<char>>, char, std::locale const&) const::'lambda'(char*, unsigned long)&>(unsigned long, std::__format::__formatter_fp<char>::_M_localize(std::basic_string_view<char, std::char_traits<char>>, char, std::locale const&) const::'lambda'(char*, unsigned long)&)::_Terminator::~_Terminator()(
        arg1 - 0x30);
    void* fsbase;
    
    if (*(arg1 - 0x18) != *(fsbase + 0x28))
    {
        __stack_chk_fail();
        /* no return */
    }
    
    _Unwind_Resume();
    __stack_chk_fail();
    /* no return */
}

char* __normal_iterator<char*, std::span<char, 18446744073709551615ul> >::operator++(int32_t arg1)
{
    int64_t* var_30 = arg1;
    int32_t rsi;
    int32_t var_34 = rsi;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t rax_2 = *var_30;
    *var_30 = rax_2 + 1;
    int64_t var_20 = rax_2;
    char* result;
    __normal_iterator<char*, std::span<char, 18446744073709551615ul> >::__normal_iterator(&result);
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

int64_t __normal_iterator<char*, std::span<char, 18446744073709551615ul> >::operator*()
{
    int64_t* rdi;
    return *rdi;
}

int64_t std::remove_reference<double>::type&& std::move<std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<double, std::__format::_Sink_iter<char>>(double, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)&>(double&&)(int64_t arg1) __pure
{
    return arg1;
}

int64_t std::remove_reference<long double>::type&& std::move<std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<long double, std::__format::_Sink_iter<char>>(long double, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)&>(long double&&)(int64_t arg1) __pure
{
    return arg1;
}

void* std::__unicode::__v15_1_0::_Grapheme_cluster_iterator_base::_Grapheme_cluster_iterator_base()
{
    void* result;
    *(result + 8) = 0;
    *(result + 9) = 0;
    *(result + 0xa) = 0;
    return result;
}

int64_t std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::_Iterator::_Iterator(std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl> arg1)
{
    int32_t* var_20 = arg1;
    std::__unicode::__v15_1_0::_Grapheme_cluster_iterator_base::_Grapheme_cluster_iterator_base();
    int64_t arg_8;
    *(var_20 + 0x10) = arg_8;
    int64_t arg_10;
    *(var_20 + 0x18) = arg_10;
    int64_t arg_18;
    *(var_20 + 0x20) = arg_18;
    int64_t arg_20;
    *(var_20 + 0x28) = arg_20;
    int64_t arg_28;
    *(var_20 + 0x30) = arg_28;
    char* rax_2 = std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::end();
    int64_t var_58 = arg_8;
    int64_t var_50 = arg_10;
    int64_t var_48 = arg_18;
    int64_t var_40 = arg_20;
    int64_t var_38 = arg_28;
    int32_t result = std::__unicode::operator==(rax_2, rax_2) ^ 1;
    
    if (result)
    {
        *var_20 = std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::operator*();
        result = std::__unicode::__v15_1_0::__grapheme_cluster_break_property(*var_20);
        var_20[1] = result;
    }
    
    return result;
}

int64_t std::__unicode::operator==(std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl> arg1, char const* arg2)
{
    int64_t result;
    result = arg1 == *std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_curr();
    return result;
}

uint64_t std::__unicode::__v15_1_0::_Grapheme_cluster_view<std::basic_string_view<char> >::_Iterator::_M_is_break(std::__unicode::__v15_1_0::_Gcb_property arg1, std::__unicode::__v15_1_0::_Gcb_property arg2, std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl> arg3)
{
    int32_t* var_60 = arg1;
    int32_t var_64 = arg2;
    int32_t var_68 = arg3;
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    uint64_t result;
    
    if (var_64 == 1 || var_64 == 2)
        result = 1;
    else if (var_64 == 3)
        result = var_68 != 2;
    else if (var_68 == 1 || var_68 == 3 || var_68 == 2)
        result = 1;
    else if (var_64 != 7)
    {
        if (var_64 == 0xb || var_64 == 8)
        {
            if (var_68 - 8 > 1)
                result = 1;
            else
                result = 0;
        }
        else if (var_64 == 0xc || var_64 == 9)
            result = var_68 != 9;
        else if (var_68 == 4 || var_68 == 0xa)
            result = 0;
        else if (var_68 == 6)
            result = 0;
        else if (var_64 != 5)
        {
            char rax_12;
            
            if (!*(var_60 + 0xa))
                rax_12 = 0;
            else if (std::__unicode::__v15_1_0::__incb_property(*var_60) != 1)
                rax_12 = 0;
            else if (std::__unicode::__v15_1_0::__incb_property(std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::operator*())
                    != 1)
                rax_12 = 0;
            else
                rax_12 = 1;
            
            char rax_24;
            
            if (rax_12)
            {
                char var_4d_1 = 0;
                int64_t rcx_1 = *(var_60 + 0x10);
                int64_t rbx_1 = *(var_60 + 0x18);
                int64_t rcx_2 = *(var_60 + 0x20);
                int64_t rbx_2 = *(var_60 + 0x28);
                int64_t rax_14 = *(var_60 + 0x30);
                int64_t arg_8;
                int64_t arg_10;
                int64_t arg_18;
                int64_t arg_20;
                int64_t arg_28;
                char rsi;
                char rdi_4;
                
                while (true)
                {
                    int64_t* rax_19;
                    char rdi_5;
                    rax_19 = std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::operator++();
                    int64_t var_90_1 = arg_8;
                    int64_t var_88_1 = arg_10;
                    int64_t var_80_1 = arg_18;
                    int64_t var_78_1 = arg_20;
                    int64_t var_70_1 = arg_28;
                    int64_t var_b8_1 = *rax_19;
                    int64_t var_b0_1 = rax_19[1];
                    int64_t var_a8_1 = rax_19[2];
                    int64_t var_a0_1 = rax_19[3];
                    int64_t var_98_1 = rax_19[4];
                    char rax_21;
                    rax_21 = std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::friend operator==(std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>, std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>)(
                        rdi_5, rsi);
                    
                    if (rax_21 == 1)
                        break;
                    
                    if (!std::__unicode::__v15_1_0::__is_incb_linker(std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::operator*()))
                    {
                        int32_t rax_18;
                        rax_18 = std::__unicode::__v15_1_0::__incb_property(std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::operator*());
                        
                        if (rax_18 != 1)
                        {
                            if (rax_18 != 2)
                                break;
                        }
                        else
                            var_4d_1 = 0;
                    }
                    else
                        var_4d_1 = 1;
                }
                
                int64_t var_90_2 = arg_8;
                int64_t var_88_2 = arg_10;
                int64_t var_80_2 = arg_18;
                int64_t var_78_2 = arg_20;
                int64_t var_70_2 = arg_28;
                int64_t var_b8_2 = rcx_1;
                int64_t var_b0_2 = rbx_1;
                int64_t var_a8_2 = rcx_2;
                int64_t var_a0_2 = rbx_2;
                int64_t var_98_2 = rax_14;
                
                if (!std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::friend operator==(std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>, std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>)(
                        rdi_4, rsi) || !var_4d_1)
                    rax_24 = 0;
                else
                    rax_24 = 1;
            }
            
            if (rax_12 && rax_24)
                result = 0;
            else if (var_64 == 0xa && var_60[2] == 2)
                result = 0;
            else if (var_64 != 0xd || var_64 != var_68)
                result = 1;
            else
                result = !(*(var_60 + 9) & 1);
        }
        else
            result = 0;
    }
    else if (var_68 > 8)
    {
        if (var_68 - 0xb > 1)
            result = 1;
        else
            result = 0;
    }
    else if (var_68 >= 7)
        result = 0;
    else
        result = 1;
    
    *(fsbase + 0x28);
    
    if (rax == *(fsbase + 0x28))
        return result;
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::_M_read_utf32()
{
    void* fsbase;
    int64_t rax = *(fsbase + 0x28);
    int64_t rdi;
    int64_t var_28 = rdi;
    int64_t var_20 = *std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::_M_curr();
    int64_t* rax_6 = std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::_M_curr();
    int32_t* rdx = *rax_6;
    *rax_6 = &rdx[1];
    char32_t var_2c = *rdx;
    
    if (std::__unicode::__is_scalar_value(var_2c) != 1)
    {
        std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::_S_error();
        var_2c = 0xfffd;
    }
    
    std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::_M_update(
        rdi, var_2c);
    std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::_Guard<char32_t const*>::~_Guard();
    
    if (rax == *(fsbase + 0x28))
        return rax - *(fsbase + 0x28);
    
    __stack_chk_fail();
    /* no return */
}

int64_t std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::friend operator==(std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>, std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>)(char arg1, char arg2)
{
    if (*std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::_M_curr()
            == *std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::_M_curr()
            && arg1 == arg2)
        return 1;
    
    return 0;
}

uint64_t std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::operator*()
{
    uint64_t rdi;
    *(rdi + 0x18);
    return *std::array<char, 4ul>::operator[](rdi);
}

void* std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::operator++()
{
    int64_t rdx_1;
    void* result;
    
    if (*(result + 0x18) + 1 == *(result + 0x19))
        rdx_1 = *std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::_M_curr();
    
    char rax_10;
    
    if (*(result + 0x18) + 1 != *(result + 0x19) || rdx_1 == *(result + 0x20))
        rax_10 = 0;
    else
        rax_10 = 1;
    
    if (rax_10)
    {
        char rax_12 = *(result + 0x1a);
        char32_t** rax_14 = std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::_M_curr();
        uint64_t rax_15 = rax_12;
        char32_t** var_20_1 = rax_14;
        std::__advance<char32_t const*, int64_t>(rax_14, rax_15, rax_15);
        int64_t rax_21;
        rax_21 = *std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::_M_curr()
            == *(result + 0x20);
        
        if (!rax_21)
            std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::_M_read();
        else
            *(result + 0x18) = 0;
    }
    else if (*(result + 0x18) + 1 < *(result + 0x19))
        *(result + 0x18) += 1;
    
    return result;
}

int64_t std::remove_reference<_Float128>::type&& std::move<std::basic_format_context<std::__format::_Sink_iter<char>, char>::iterator std::__format::__formatter_fp<char>::format<_Float128, std::__format::_Sink_iter<char>>(_Float128, std::basic_format_context<std::__format::_Sink_iter<char>, char>&) const::'lambda0'(char*, unsigned long)&>(_Float128&&)(int64_t arg1) __pure
{
    return arg1;
}

std::__format::__formatter_fp<char>::_M_localize(std::basic_string_view<char>, char, std::locale const&) const::'lambda'(char*, uint64_t)&&& std::move<std::__format::__formatter_fp<char>::_M_localize(std::basic_string_view<char>, char, std::locale const&) const::'lambda'(char*, uint64_t)&>(std::__format::__formatter_fp<char>::_M_localize(std::basic_string_view<char>, char, std::locale const&) const::'lambda'(char*, uint64_t)&&& arg1) __pure
{
    return arg1;
}

int64_t std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::friend operator==(std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>, std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>)(char arg1, char arg2)
{
    if (*std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_curr()
            == *std::__unicode::_Utf_iterator<char, char32_t, char const*, char const*, std::__unicode::_Repl>::_M_curr()
            && arg1 == arg2)
        return 1;
    
    return 0;
}

char32_t*** std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::_Guard<char32_t const*>::~_Guard()
{
    int64_t* rdi;
    char32_t** rbx = *std::move<char32_t const*&>(&rdi[1]);
    *rdi;
    char32_t*** result = std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::_M_curr();
    *result = rbx;
    return result;
}

int64_t std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::_S_error()
{
    int64_t rdi;
    int64_t var_20 = rdi;
    int32_t var_14 = 0xfffd;
    void* fsbase;
    *(fsbase + 0x28);
    
    if (*(fsbase + 0x28) == *(fsbase + 0x28))
        return 0xfffd;
    
    __stack_chk_fail();
    /* no return */
}

uint64_t std::__unicode::_Utf_iterator<char32_t, char, char32_t const*, char32_t const*, std::__unicode::_Repl>::_M_update(char32_t arg1, uint8_t arg2)
{
    uint64_t result_1 = arg1;
    uint32_t var_34 = arg2;
    char rdx;
    *(result_1 + 0x1a) = rdx;
    *(result_1 + 0x18) = 0;
    int32_t rax_4 = int std::bit_width<unsigned int>(unsigned int)(var_34);
    uint64_t result;
    
    if (rax_4 <= 7)
    {
        *std::array<char, 4ul>::operator[](result_1) = var_34;
        char* rax_9 = std::array<char, 4ul>::operator[](result_1);
        *rax_9 = 0;
        char rbx_1 = *rax_9;
        char* rax_11 = std::array<char, 4ul>::operator[](result_1);
        *rax_11 = rbx_1;
        *std::array<char, 4ul>::operator[](result_1) = *rax_11;
        result = result_1;
        *(result + 0x19) = 1;
    }
    else if (rax_4 <= 0xb)
    {
        *std::array<char, 4ul>::operator[](result_1) = var_34 >> 6 | 0xc0;
        *std::array<char, 4ul>::operator[](result_1) = (var_34 & 0x3f) | 0x80;
        char* rax_25 = std::array<char, 4ul>::operator[](result_1);
        *rax_25 = 0;
        *std::array<char, 4ul>::operator[](result_1) = *rax_25;
        result = result_1;
        *(result + 0x19) = 2;
    }
    else if (rax_4 > 0x10)
    {
        *std::array<char, 4ul>::operator[](result_1) = (var_34 >> 0x12 & 7) | 0xf0;
        *std::array<char, 4ul>::operator[](result_1) = (var_34 >> 0xc & 0x3f) | 0x80;
        *std::array<char, 4ul>::operator[](result_1) = (var_34 >> 6 & 0x3f) | 0x80;
        *std::array<char, 4ul>::operator[](result_1) = (var_34 & 0x3f) | 0x80;
        result = result_1;
        *(result + 0x19) = 4;
    }
    else
    {
        *std::array<char, 4ul>::operator[](result_1) = var_34 >> 0xc | 0xe0;
        *std::array<char, 4ul>::operator[](result_1) = (var_34 >> 6 & 0x3f) | 0x80;
        *std::array<char, 4ul>::operator[](result_1) = (var_34 & 0x3f) | 0x80;
        *std::array<char, 4ul>::operator[](result_1) = 0;
        result = result_1;
        *(result + 0x19) = 3;
    }
    
    return result;
}

int64_t std::array<char, 4ul>::operator[](uint64_t arg1) __pure
{
    int64_t rsi;
    return rsi + arg1;
}

char32_t const*&&& std::move<char32_t const*&>(char32_t const*&&& arg1) __pure
{
    return arg1;
}

int32_t int std::bit_width<unsigned int>(unsigned int)(uint32_t arg1)
{
    return std::__bit_width<uint32_t>(arg1);
}

int64_t std::array<char, 4ul>::operator[](uint64_t arg1) __pure
{
    int64_t rsi;
    return rsi + arg1;
}

