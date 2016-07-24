#define UTIX_EXCEPTIONS_H_

#if __EXCEPTIONS
#define UTIX_HAS_EXCEPTIONS_ 1
#define UTIX_NOEXCEPT_ noexcept
#define UTIX_TRY_(try_code) try { try_code }
#define UTIX_CATCH_(exception, handle_code) catch(exception) { handle_code }
#else
#define UTIX_NOEXCEPT_
#define UTIX_TRY_(try_code) try_code
#define UTIX_CATCH_(exception, handle_code)
#endif
