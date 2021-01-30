#pragma once

#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/spirit/include/qi.hpp>
#include "ConfigurationParser.hpp"

BOOST_FUSION_ADAPT_STRUCT(Owl::ConfInfo, rules)
BOOST_FUSION_ADAPT_STRUCT(Owl::RuleInfo, type, value, policy)

namespace qi = boost::spirit::qi;
using qi::eol;
using qi::eoi;
using qi::alpha;
using qi::char_;
using qi::alnum;
using qi::raw;
using qi::hold;

namespace Owl {
    /**
     *
     * @tparam Iterator
     * @tparam Skipper
     *
     * @note hold[] directive guarantees that only the succeeding alternatives gets to modify the attribute.
     * For more details: https://www.boost.org/doc/libs/1_74_0/libs/spirit/doc/html/spirit/qi/reference/directive/hold.html
     */
    template<typename Iterator, typename Skipper>
    struct ConfigurationGrammar : public qi::grammar<Iterator, Owl::ConfInfo(), Skipper> {
        ConfigurationGrammar() : ConfigurationGrammar::base_type(conf) {
            conf %= rules;

            rules %= "[Rule]" >> +eol >> *rule;
            rule %= ruleType >> ',' >> -hold[ruleValue >> ','] >> name >> termination;
            ruleType %= +(alpha | char_("-_"));
            ruleValue %= +(alnum | char_(".-_!/"));

            name %= raw[+(char_ - eol - char_("=,"))];
            property %= key >> "=" >> value;
            key %= +(alpha | char_("-_"));
            value %= +(alnum | char_("-_.!"));
            termination %= +eol | eoi;
        }

        qi::rule<Iterator, ConfInfo(), Skipper> conf;

        qi::rule<Iterator, std::vector<RuleInfo>(), Skipper> rules;
        qi::rule<Iterator, RuleInfo(), Skipper> rule;
        qi::rule<Iterator, std::u32string()> ruleType;
        qi::rule<Iterator, std::u32string()> ruleValue;

        qi::rule<Iterator, std::u32string()> name;
        qi::rule<Iterator, std::pair<std::u32string, std::u32string>()> property;
        qi::rule<Iterator, std::u32string()> key;
        qi::rule<Iterator, std::u32string()> value;
        qi::rule<Iterator> termination;
    };
}


