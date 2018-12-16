#include "Decoder.hpp"
#include "Request.hpp"

using namespace Server::Network::Connection::Manager;

Decoder::Decoder()
	: _state(State::method_start)
{
}

void Decoder::reset()
{
	_state = State::method_start;
}

Decoder::result_type Decoder::consume(Request& req, char input)
{
	switch (_state)
	{
	case State::method_start:
		if (!is_char(input) || is_ctl(input) || is_tspecial(input))
		{
			return bad;
		}
		else
		{
			_state = State::method;
			req._method.push_back(input);
			return indeterminate;
		}
	case State::method:
		if (input == ' ')
		{
			_state = State::uri;
			return indeterminate;
		}
		else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
		{
			return bad;
		}
		else
		{
			req._method.push_back(input);
			return indeterminate;
		}
	case State::uri:
		if (input == ' ')
		{
			_state = State::http_version_h;
			return indeterminate;
		}
		else if (is_ctl(input))
		{
			return bad;
		}
		else
		{
			req._uri.push_back(input);
			return indeterminate;
		}
	case State::http_version_h:
		if (input == 'H')
		{
			_state = State::http_version_t_1;
			return indeterminate;
		}
		else
		{
			return bad;
		}
	case State::http_version_t_1:
		if (input == 'T')
		{
			_state = State::http_version_t_2;
			return indeterminate;
		}
		else
		{
			return bad;
		}
	case State::http_version_t_2:
		if (input == 'T')
		{
			_state = State::http_version_p;
			return indeterminate;
		}
		else
		{
			return bad;
		}
	case State::http_version_p:
		if (input == 'P')
		{
			_state = State::http_version_slash;
			return indeterminate;
		}
		else
		{
			return bad;
		}
	case State::http_version_slash:
		if (input == '/')
		{
			req._http_version_major = 0;
			req._http_version_minor = 0;
			_state = State::http_version_major_start;
			return indeterminate;
		}
		else
		{
			return bad;
		}
	case State::http_version_major_start:
		if (is_digit(input))
		{
			req._http_version_major = req._http_version_major * 10 + input - '0';
			_state = State::http_version_major;
			return indeterminate;
		}
		else
		{
			return bad;
		}
	case State::http_version_major:
		if (input == '.')
		{
			_state = State::http_version_minor_start;
			return indeterminate;
		}
		else if (is_digit(input))
		{
			req._http_version_major = req._http_version_major * 10 + input - '0';
			return indeterminate;
		}
		else
		{
			return bad;
		}
	case State::http_version_minor_start:
		if (is_digit(input))
		{
			req._http_version_minor = req._http_version_minor * 10 + input - '0';
			_state = State::http_version_minor;
			return indeterminate;
		}
		else
		{
			return bad;
		}
	case State::http_version_minor:
		if (input == '\r')
		{
			_state = State::expecting_newline_1;
			return indeterminate;
		}
		else if (is_digit(input))
		{
			req._http_version_minor = req._http_version_minor * 10 + input - '0';
			return indeterminate;
		}
		else
		{
			return bad;
		}
	case State::expecting_newline_1:
		if (input == '\n')
		{
			_state = State::header_line_start;
			return indeterminate;
		}
		else
		{
			return bad;
		}
	case State::header_line_start:
		if (input == '\r')
		{
			_state = State::expecting_newline_3;
			return indeterminate;
		}
		else if (!req._headers.empty() && (input == ' ' || input == '\t'))
		{
			_state = State::header_lws;
			return indeterminate;
		}
		else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
		{
			return bad;
		}
		else
		{
			req._headers.push_back(Header());
			req._headers.back().name.push_back(input);
			_state = State::header_name;
			return indeterminate;
		}
	case State::header_lws:
		if (input == '\r')
		{
			_state = State::expecting_newline_2;
			return indeterminate;
		}
		else if (input == ' ' || input == '\t')
		{
			return indeterminate;
		}
		else if (is_ctl(input))
		{
			return bad;
		}
		else
		{
			_state = State::header_value;
			req._headers.back().value.push_back(input);
			return indeterminate;
		}
	case State::header_name:
		if (input == ':')
		{
			_state = State::space_before_header_value;
			return indeterminate;
		}
		else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
		{
			return bad;
		}
		else
		{
			req._headers.back().name.push_back(input);
			return indeterminate;
		}
	case State::space_before_header_value:
		if (input == ' ')
		{
			_state = State::header_value;
			return indeterminate;
		}
		else
		{
			return bad;
		}
	case State::header_value:
		if (input == '\r')
		{
			_state = State::expecting_newline_2;
			return indeterminate;
		}
		else if (is_ctl(input))
		{
			return bad;
		}
		else
		{
			req._headers.back().value.push_back(input);
			return indeterminate;
		}
	case State::expecting_newline_2:
		if (input == '\n')
		{
			_state = State::header_line_start;
			return indeterminate;
		}
		else
		{
			return bad;
		}
	case State::expecting_newline_3:
		return (input == '\n') ? good : bad;
	default:
		return bad;
	}
}

bool Decoder::is_char(int c)
{
	return c >= 0 && c <= 127;
}

bool Decoder::is_ctl(int c)
{
	return (c >= 0 && c <= 31) || (c == 127);
}

bool Decoder::is_tspecial(int c)
{
	switch (c)
	{
	case '(': case ')': case '<': case '>': case '@':
	case ',': case ';': case ':': case '\\': case '"':
	case '/': case '[': case ']': case '?': case '=':
	case '{': case '}': case ' ': case '\t':
		return true;
	default:
		return false;
	}
}

bool Decoder::is_digit(int c)
{
	return c >= '0' && c <= '9';
}