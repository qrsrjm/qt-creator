/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**************************************************************************/
#include <namedemangler/namedemangler.h>
#include <namedemangler/parsetreenodes.h>

#include <QObject>
#include <QTest>

#define TEST_CORRECTLY_MANGLED_NAME(mangled, expectedDemangled) \
    do { \
        QVERIFY2(demangler.demangle(mangled), qPrintable(demangler.errorString())); \
        QCOMPARE(demangler.demangledName(), QLatin1String(expectedDemangled)); \
    } while (0)

using namespace Debugger::Internal;

class NameDemanglerAutoTest : public QObject
{
    Q_OBJECT
private slots:
    void testUnmangledName();
    void testDisjunctFirstSets();
    void TEST_CORRECTLY_MANGLED_NAMEs();
    void testIncorrectlyMangledNames();

private:
    void testIncorrectlyMangledName(const QString &mangledName);
    NameDemangler demangler;
};

void NameDemanglerAutoTest::testUnmangledName()
{
    QVERIFY(demangler.demangle("f") && demangler.demangledName() == "f");
}

void NameDemanglerAutoTest::TEST_CORRECTLY_MANGLED_NAMEs()
{
    TEST_CORRECTLY_MANGLED_NAME("_Z1fv", "f()");
    TEST_CORRECTLY_MANGLED_NAME("_Z1fi", "f(int)");
    TEST_CORRECTLY_MANGLED_NAME("_Z3foo3bar", "foo(bar)");
    TEST_CORRECTLY_MANGLED_NAME("_Zrm1XS_", "operator%(X, X)");
    TEST_CORRECTLY_MANGLED_NAME("_ZplR1XS0_", "operator+(X &, X &)");
    TEST_CORRECTLY_MANGLED_NAME("_ZlsRK1XS1_", "operator<<(X const &, X const &)");
    TEST_CORRECTLY_MANGLED_NAME("_ZN3FooIA4_iE3barE", "Foo<int[4]>::bar");
    TEST_CORRECTLY_MANGLED_NAME("_Z1fIiEvi", "void f<int>(int)");
    TEST_CORRECTLY_MANGLED_NAME("_Z5firstI3DuoEvS0_", "void first<Duo>(Duo)");
    TEST_CORRECTLY_MANGLED_NAME("_Z5firstI3DuoEvT_", "void first<Duo>(Duo)");
    TEST_CORRECTLY_MANGLED_NAME("_Z3fooIiPFidEiEvv",
                             "void foo<int, int (*)(double), int>()");
    TEST_CORRECTLY_MANGLED_NAME("_ZN1N1fE", "N::f");
    TEST_CORRECTLY_MANGLED_NAME("_ZN6System5Sound4beepEv",
                             "System::Sound::beep()");
    TEST_CORRECTLY_MANGLED_NAME("_ZN5Arena5levelE", "Arena::level");
    TEST_CORRECTLY_MANGLED_NAME("_ZN5StackIiiE5levelE", "Stack<int, int>::level");
    TEST_CORRECTLY_MANGLED_NAME("_Z1fI1XEvPVN1AIT_E1TE",
                             "void f<X>(A<X>::T volatile *)");
    TEST_CORRECTLY_MANGLED_NAME("_ZngILi42EEvN1AIXplT_Li2EEE1TE",
                             "void operator-<42>(A<42 + 2>::T)");
    TEST_CORRECTLY_MANGLED_NAME("_Z4makeI7FactoryiET_IT0_Ev",
                             "Factory<int> make<Factory, int>()");
    TEST_CORRECTLY_MANGLED_NAME("_Z3foo5Hello5WorldS0_S_",
                             "foo(Hello, World, World, Hello)");
    TEST_CORRECTLY_MANGLED_NAME("_Z3fooPM2ABi", "foo(int AB::**)");
    TEST_CORRECTLY_MANGLED_NAME("_ZlsRSoRKSs",
        "operator<<(std::basic_ostream<char, std::char_traits<char> > &, "
        "std::basic_string<char, std::char_traits<char>, "
        "std::allocator<char> > const &)");
    TEST_CORRECTLY_MANGLED_NAME("_ZTI7a_class", "typeid(a_class)");
    TEST_CORRECTLY_MANGLED_NAME("_ZZN1A3fooEiE1B", "A::foo(int)::B");
    TEST_CORRECTLY_MANGLED_NAME("_ZZ3foovEN1C1DE", "foo()::C::D");
    TEST_CORRECTLY_MANGLED_NAME("_ZZZ3foovEN1C3barEvEN1E3bazEv",
                             "foo()::C::bar()::E::baz()");
    TEST_CORRECTLY_MANGLED_NAME("_ZZN1N1fEiE1p", "N::f(int)::p");
    TEST_CORRECTLY_MANGLED_NAME("_ZZN1N1fEiEs", "N::f(int)::[string literal]");
    TEST_CORRECTLY_MANGLED_NAME("_Z3fooc", "foo(char)");
    TEST_CORRECTLY_MANGLED_NAME("_Z2CBIL_Z3foocEE", "CB<foo(char)>");
    TEST_CORRECTLY_MANGLED_NAME("_Z2CBIL_Z7IsEmptyEE", "CB<IsEmpty>");
    TEST_CORRECTLY_MANGLED_NAME("_ZN1N1TIiiE2mfES0_IddE",
                             "N::T<int, int>::mf(N::T<double, double>)");
    TEST_CORRECTLY_MANGLED_NAME("_ZSt5state", "std::state");
    TEST_CORRECTLY_MANGLED_NAME("_ZNSt3_In4wardE", "std::_In::ward");
    TEST_CORRECTLY_MANGLED_NAME("_Z41__static_initialization_and_destruction_0ii",
        "__static_initialization_and_destruction_0(int, int)");
    TEST_CORRECTLY_MANGLED_NAME("_ZN20NameDemanglerPrivate3eoiE",
                             "NameDemanglerPrivate::eoi");
    TEST_CORRECTLY_MANGLED_NAME(
        "_ZZN20NameDemanglerPrivate15parseIdentifierEiE8__func__",
        "NameDemanglerPrivate::parseIdentifier(int)::__func__");
    TEST_CORRECTLY_MANGLED_NAME("_ZN4QSetI5QCharED1Ev", "QSet<QChar>::~QSet()");
    TEST_CORRECTLY_MANGLED_NAME("_Zne5QCharS_", "operator!=(QChar, QChar)");
    TEST_CORRECTLY_MANGLED_NAME("_ZN20NameDemanglerPrivate17parseFunctionTypeEv",
                             "NameDemanglerPrivate::parseFunctionType()");
    TEST_CORRECTLY_MANGLED_NAME(
        "_ZNK20NameDemanglerPrivate16ArrayNewOperator8makeExprERK11QStringList",
        "NameDemanglerPrivate::ArrayNewOperator::makeExpr(QStringList const &) const");
    TEST_CORRECTLY_MANGLED_NAME("_ZN13QLatin1StringC1EPKc",
                             "QLatin1String::QLatin1String(char const *)");
    TEST_CORRECTLY_MANGLED_NAME(
        "_ZN15QtSharedPointer16ExternalRefCountIN20NameDemanglerPrivate8OperatorEE12internalCopyIS2_EEvRKNS0_IT_EE",
        "void QtSharedPointer::ExternalRefCount<NameDemanglerPrivate::Operator>::internalCopy<NameDemanglerPrivate::Operator>(QtSharedPointer::ExternalRefCount<NameDemanglerPrivate::Operator> const &)");
    TEST_CORRECTLY_MANGLED_NAME(
        "_ZN15QtSharedPointer16ExternalRefCountIN20NameDemanglerPrivate8OperatorEE11internalSetEPNS_20ExternalRefCountDataEPS2_",
        "QtSharedPointer::ExternalRefCount<NameDemanglerPrivate::Operator>::internalSet(QtSharedPointer::ExternalRefCountData *, NameDemanglerPrivate::Operator *)");
    TEST_CORRECTLY_MANGLED_NAME("_ZN20NameDemanglerPrivate17parseUnscopedNameEv",
                             "NameDemanglerPrivate::parseUnscopedName()");
    TEST_CORRECTLY_MANGLED_NAME("_ZNK7QString3argExiiRK5QChar",
        "QString::arg(long long, int, int, QChar const &) const");
    TEST_CORRECTLY_MANGLED_NAME(
        "_ZN20NameDemanglerPrivate8OperatorC2ERK7QStringS3_",
        "NameDemanglerPrivate::Operator::Operator(QString const &, QString const &)");
    TEST_CORRECTLY_MANGLED_NAME(
        "_ZN15QtSharedPointer16ExternalRefCountIN20NameDemanglerPrivate8OperatorEEC2EN2Qt14InitializationE",
        "QtSharedPointer::ExternalRefCount<NameDemanglerPrivate::Operator>::ExternalRefCount(Qt::Initialization)");
    TEST_CORRECTLY_MANGLED_NAME("_ZN7QString5clearEv", "QString::clear()");
    TEST_CORRECTLY_MANGLED_NAME("_ZNK5QListI7QStringE2atEi",
                             "QList<QString>::at(int) const");
    TEST_CORRECTLY_MANGLED_NAME(
        "_ZNK7QString10startsWithERKS_N2Qt15CaseSensitivityE",
        "QString::startsWith(QString const &, Qt::CaseSensitivity) const");
    TEST_CORRECTLY_MANGLED_NAME("_ZNK4QSetI5QCharE8constEndEv",
                             "QSet<QChar>::constEnd() const");
    TEST_CORRECTLY_MANGLED_NAME("_Z11qt_assert_xPKcS0_S0_i",
        "qt_assert_x(char const *, char const *, char const *, int)");
    TEST_CORRECTLY_MANGLED_NAME("_ZN9QHashData8willGrowEv",
                             "QHashData::willGrow()");
    TEST_CORRECTLY_MANGLED_NAME(
        "_ZNK5QHashI5QChar15QHashDummyValueE14const_iteratorneERKS3_",
        "QHash<QChar, QHashDummyValue>::const_iterator::operator!=(QHash<QChar, QHashDummyValue>::const_iterator const &) const");
    TEST_CORRECTLY_MANGLED_NAME("_ZNK13NameDemangler11errorStringEv",
                             "NameDemangler::errorString() const");
    TEST_CORRECTLY_MANGLED_NAME("_ZN7QString7replaceERK7QRegExpRKS_",
        "QString::replace(QRegExp const &, QString const &)");
    TEST_CORRECTLY_MANGLED_NAME("_ZN7QString4freeEPNS_4DataE",
                             "QString::free(QString::Data *)");
    TEST_CORRECTLY_MANGLED_NAME(
        "_ZTSN20NameDemanglerPrivate19ArrayAccessOperatorE",
        "typeid(NameDemanglerPrivate::ArrayAccessOperator).name()");
    TEST_CORRECTLY_MANGLED_NAME("_ZN3ns11fERKPFPKiS1_RKhE",
        "ns1::f(int const * (* const &)(int const *, unsigned char const &))");
    TEST_CORRECTLY_MANGLED_NAME("_Z9test_funcMN3ns11cImEEKFPKvPiRlmE",
        "test_func(void const * (ns1::c<unsigned long>::*)(int *, long &, unsigned long) const)");
    TEST_CORRECTLY_MANGLED_NAME("_ZN3ns11fEPKPFPKiS1_RKhE",
        "ns1::f(int const * (* const *)(int const *, unsigned char const &))");
    TEST_CORRECTLY_MANGLED_NAME("_ZNK1CcviEv", "C::operator int() const");
    TEST_CORRECTLY_MANGLED_NAME("_ZN1CppEv", "C::operator++()");
    TEST_CORRECTLY_MANGLED_NAME("_ZN1CmmEv", "C::operator--()");
    TEST_CORRECTLY_MANGLED_NAME("_ZN1CppEi", "C::operator++(int)");
    TEST_CORRECTLY_MANGLED_NAME("_ZN1CmmEi", "C::operator--(int)");
    TEST_CORRECTLY_MANGLED_NAME("_ZNK1CcvT_IPKcEEv", "C::operator char const *<char const *>() const");
}

void NameDemanglerAutoTest::testIncorrectlyMangledNames()
{
}

void NameDemanglerAutoTest::testDisjunctFirstSets()
{
    for (char c = 0x20; c < 0x7e; ++c) {

        // <encoding>
        QVERIFY(!NameNode::mangledRepresentationStartsWith(c)
                || !SpecialNameNode::mangledRepresentationStartsWith(c));

        // <name>
        QVERIFY(!NestedNameNode::mangledRepresentationStartsWith(c)
                || !UnscopedNameNode::mangledRepresentationStartsWith(c));
        QVERIFY(!NestedNameNode::mangledRepresentationStartsWith(c)
                || !SubstitutionNode::mangledRepresentationStartsWith(c));
        QVERIFY(!NestedNameNode::mangledRepresentationStartsWith(c)
                || !LocalNameNode::mangledRepresentationStartsWith(c));
        QVERIFY(!UnscopedNameNode::mangledRepresentationStartsWith(c)
                || !SubstitutionNode::mangledRepresentationStartsWith(c) || c == 'S');
        QVERIFY(!UnscopedNameNode::mangledRepresentationStartsWith(c)
                || !LocalNameNode::mangledRepresentationStartsWith(c));
        QVERIFY(!SubstitutionNode::mangledRepresentationStartsWith(c)
                || !LocalNameNode::mangledRepresentationStartsWith(c));

        // <nested-name>
        QVERIFY(!CvQualifiersNode::mangledRepresentationStartsWith(c)
                || !PrefixNode::mangledRepresentationStartsWith(c) || c == 'r');

        // <prefix>
        QVERIFY(!TemplateParamNode::mangledRepresentationStartsWith(c)
                || !SubstitutionNode::mangledRepresentationStartsWith(c));
        QVERIFY(!TemplateArgsNode::mangledRepresentationStartsWith(c)
                || !Prefix2Node::mangledRepresentationStartsWith(c));
        QVERIFY(!TemplateParamNode::mangledRepresentationStartsWith(c)
                || !Prefix2Node::mangledRepresentationStartsWith(c));
        QVERIFY(!SubstitutionNode::mangledRepresentationStartsWith(c)
                || !Prefix2Node::mangledRepresentationStartsWith(c));

        // <prefix2>
        QVERIFY(!TemplateArgsNode::mangledRepresentationStartsWith(c)
                || !Prefix2Node::mangledRepresentationStartsWith(c));


        // <template-arg>
        QVERIFY(!TypeNode::mangledRepresentationStartsWith(c)
                || !ExprPrimaryNode::mangledRepresentationStartsWith(c));

        // <expression>
        QVERIFY(!OperatorNameNode::mangledRepresentationStartsWith(c)
                || !TemplateParamNode::mangledRepresentationStartsWith(c));
//        QVERIFY(!OperatorNameNode::mangledRepresentationStartsWith(c)
//                || !FunctionParamNode::mangledRepresentationStartsWith(c))
        QVERIFY(!OperatorNameNode::mangledRepresentationStartsWith(c)
                || !ExprPrimaryNode::mangledRepresentationStartsWith(c));
//        QVERIFY(!TemplateParamNode::mangledRepresentationStartsWith(c)
//                || !FunctionParamNode::mangledRepresentationStartsWith(c));
        QVERIFY(!TemplateParamNode::mangledRepresentationStartsWith(c)
                || !ExprPrimaryNode::mangledRepresentationStartsWith(c));
//        QVERIFY(!FunctionParamNode::mangledRepresentationStartsWith(c)
//                || !ExprPrimaryNode::mangledRepresentationStartsWith(c));

        // <expr-primary>
        QVERIFY(!TypeNode::mangledRepresentationStartsWith(c)
                || !MangledNameNode::mangledRepresentationStartsWith(c));

        // <type>
        QVERIFY(!BuiltinTypeNode::mangledRepresentationStartsWith(c)
                || !FunctionTypeNode::mangledRepresentationStartsWith(c));

        QVERIFY2(!BuiltinTypeNode::mangledRepresentationStartsWith(c)
                || !ClassEnumTypeNode::mangledRepresentationStartsWith(c) || c == 'D', &c);
        QVERIFY(!BuiltinTypeNode::mangledRepresentationStartsWith(c)
                || !ArrayTypeNode::mangledRepresentationStartsWith(c));
        QVERIFY(!BuiltinTypeNode::mangledRepresentationStartsWith(c)
                || !PointerToMemberTypeNode::mangledRepresentationStartsWith(c));
        QVERIFY(!BuiltinTypeNode::mangledRepresentationStartsWith(c)
                || !TemplateParamNode::mangledRepresentationStartsWith(c));
        QVERIFY(!BuiltinTypeNode::mangledRepresentationStartsWith(c)
                || !SubstitutionNode::mangledRepresentationStartsWith(c));
        QVERIFY(!BuiltinTypeNode::mangledRepresentationStartsWith(c)
                || !CvQualifiersNode::mangledRepresentationStartsWith(c));
        QVERIFY(!FunctionTypeNode::mangledRepresentationStartsWith(c)
                || !ClassEnumTypeNode::mangledRepresentationStartsWith(c));
        QVERIFY(!FunctionTypeNode::mangledRepresentationStartsWith(c)
                || !ArrayTypeNode::mangledRepresentationStartsWith(c));
        QVERIFY(!FunctionTypeNode::mangledRepresentationStartsWith(c)
                || !PointerToMemberTypeNode::mangledRepresentationStartsWith(c));
        QVERIFY(!FunctionTypeNode::mangledRepresentationStartsWith(c)
                || !TemplateParamNode::mangledRepresentationStartsWith(c));
        QVERIFY(!FunctionTypeNode::mangledRepresentationStartsWith(c)
                || !SubstitutionNode::mangledRepresentationStartsWith(c));
        QVERIFY(!FunctionTypeNode::mangledRepresentationStartsWith(c)
                || !CvQualifiersNode::mangledRepresentationStartsWith(c));
        QVERIFY(!ClassEnumTypeNode::mangledRepresentationStartsWith(c)
                || !ArrayTypeNode::mangledRepresentationStartsWith(c));
        QVERIFY(!ClassEnumTypeNode::mangledRepresentationStartsWith(c)
                || !PointerToMemberTypeNode::mangledRepresentationStartsWith(c));
        QVERIFY(!ClassEnumTypeNode::mangledRepresentationStartsWith(c)
                || !TemplateParamNode::mangledRepresentationStartsWith(c));
        QVERIFY(!ClassEnumTypeNode::mangledRepresentationStartsWith(c)
                || !SubstitutionNode::mangledRepresentationStartsWith(c));
        QVERIFY(!ClassEnumTypeNode::mangledRepresentationStartsWith(c)
                || !CvQualifiersNode::mangledRepresentationStartsWith(c));
        QVERIFY(!ArrayTypeNode::mangledRepresentationStartsWith(c)
                || !PointerToMemberTypeNode::mangledRepresentationStartsWith(c));
        QVERIFY(!ArrayTypeNode::mangledRepresentationStartsWith(c)
                || !TemplateParamNode::mangledRepresentationStartsWith(c));
        QVERIFY(!ArrayTypeNode::mangledRepresentationStartsWith(c)
                || !SubstitutionNode::mangledRepresentationStartsWith(c));
        QVERIFY(!ArrayTypeNode::mangledRepresentationStartsWith(c)
                || !CvQualifiersNode::mangledRepresentationStartsWith(c));
        QVERIFY(!PointerToMemberTypeNode::mangledRepresentationStartsWith(c)
                || !TemplateParamNode::mangledRepresentationStartsWith(c));
        QVERIFY(!PointerToMemberTypeNode::mangledRepresentationStartsWith(c)
                || !SubstitutionNode::mangledRepresentationStartsWith(c));
        QVERIFY(!PointerToMemberTypeNode::mangledRepresentationStartsWith(c)
                || !CvQualifiersNode::mangledRepresentationStartsWith(c));
        QVERIFY(!TemplateParamNode::mangledRepresentationStartsWith(c)
                || !SubstitutionNode::mangledRepresentationStartsWith(c));
        QVERIFY(!TemplateParamNode::mangledRepresentationStartsWith(c)
                || !CvQualifiersNode::mangledRepresentationStartsWith(c));
        QVERIFY(!SubstitutionNode::mangledRepresentationStartsWith(c)
                || !CvQualifiersNode::mangledRepresentationStartsWith(c));

        // <unqualified-name>
        QVERIFY(!OperatorNameNode::mangledRepresentationStartsWith(c)
                || !CtorDtorNameNode::mangledRepresentationStartsWith(c));
        QVERIFY(!OperatorNameNode::mangledRepresentationStartsWith(c)
                || !SourceNameNode::mangledRepresentationStartsWith(c));
        QVERIFY(!CtorDtorNameNode::mangledRepresentationStartsWith(c)
                || !SourceNameNode::mangledRepresentationStartsWith(c));

        // <array-type>
        QVERIFY(!NonNegativeNumberNode::mangledRepresentationStartsWith(c)
                || !ExpressionNode::mangledRepresentationStartsWith(c));
    }

    // <template-args>, <template-arg>
    QVERIFY(!TemplateArgNode::mangledRepresentationStartsWith('E'));

    // <template-arg>
    QVERIFY(!TypeNode::mangledRepresentationStartsWith('X')
            && !TypeNode::mangledRepresentationStartsWith('J')
            /* && !TypeNode::mangledRepresentationStartsWith('s') */);
    QVERIFY(!ExprPrimaryNode::mangledRepresentationStartsWith('X')
             && !ExprPrimaryNode::mangledRepresentationStartsWith('J')
             && !ExprPrimaryNode::mangledRepresentationStartsWith('s'));

    // <expression>
    QVERIFY(!TemplateParamNode::mangledRepresentationStartsWith('c')
            && !TemplateParamNode::mangledRepresentationStartsWith('s')
            && !TemplateParamNode::mangledRepresentationStartsWith('a'));
//    QVERIFY(!FunctionParamNode::mangledRepresentationStartsWith('c')
//            && !FunctionParamNode::mangledRepresentationStartsWith('c')
//            && !FunctionParamNode::mangledRepresentationStartsWith('c'));
    QVERIFY(!ExprPrimaryNode::mangledRepresentationStartsWith('c')
            && !ExprPrimaryNode::mangledRepresentationStartsWith('s')
            && !ExprPrimaryNode::mangledRepresentationStartsWith('a'));
    QVERIFY(!ExpressionNode::mangledRepresentationStartsWith('E'));
    QVERIFY(!ExpressionNode::mangledRepresentationStartsWith('_'));

    // <type>
    QVERIFY(!BuiltinTypeNode::mangledRepresentationStartsWith('P')
            && !BuiltinTypeNode::mangledRepresentationStartsWith('R')
            && !BuiltinTypeNode::mangledRepresentationStartsWith('O')
            && !BuiltinTypeNode::mangledRepresentationStartsWith('C')
            && !BuiltinTypeNode::mangledRepresentationStartsWith('G')
            && !BuiltinTypeNode::mangledRepresentationStartsWith('U'));
    QVERIFY(!FunctionTypeNode::mangledRepresentationStartsWith('P')
            && !FunctionTypeNode::mangledRepresentationStartsWith('R')
            && !FunctionTypeNode::mangledRepresentationStartsWith('O')
            && !FunctionTypeNode::mangledRepresentationStartsWith('C')
            && !FunctionTypeNode::mangledRepresentationStartsWith('G')
            && !FunctionTypeNode::mangledRepresentationStartsWith('U')
            && !FunctionTypeNode::mangledRepresentationStartsWith('D'));
    QVERIFY(!ClassEnumTypeNode::mangledRepresentationStartsWith('P')
            && !ClassEnumTypeNode::mangledRepresentationStartsWith('R')
            && !ClassEnumTypeNode::mangledRepresentationStartsWith('O')
            && !ClassEnumTypeNode::mangledRepresentationStartsWith('C')
            && !ClassEnumTypeNode::mangledRepresentationStartsWith('G')
            && !ClassEnumTypeNode::mangledRepresentationStartsWith('U')
            /* && !firstSetClassEnumType.contains('D') */);
    QVERIFY(!ArrayTypeNode::mangledRepresentationStartsWith('P')
            && !ArrayTypeNode::mangledRepresentationStartsWith('R')
            && !ArrayTypeNode::mangledRepresentationStartsWith('O')
            && !ArrayTypeNode::mangledRepresentationStartsWith('C')
            && !ArrayTypeNode::mangledRepresentationStartsWith('G')
            && !ArrayTypeNode::mangledRepresentationStartsWith('U')
            && !ArrayTypeNode::mangledRepresentationStartsWith('D'));
    QVERIFY(!PointerToMemberTypeNode::mangledRepresentationStartsWith('P')
            && !PointerToMemberTypeNode::mangledRepresentationStartsWith('R')
            && !PointerToMemberTypeNode::mangledRepresentationStartsWith('O')
            && !PointerToMemberTypeNode::mangledRepresentationStartsWith('C')
            && !PointerToMemberTypeNode::mangledRepresentationStartsWith('G')
            && !PointerToMemberTypeNode::mangledRepresentationStartsWith('U')
            && !PointerToMemberTypeNode::mangledRepresentationStartsWith('D'));
    QVERIFY(!TemplateParamNode::mangledRepresentationStartsWith('P')
            && !TemplateParamNode::mangledRepresentationStartsWith('R')
            && !TemplateParamNode::mangledRepresentationStartsWith('O')
            && !TemplateParamNode::mangledRepresentationStartsWith('C')
            && !TemplateParamNode::mangledRepresentationStartsWith('G')
            && !TemplateParamNode::mangledRepresentationStartsWith('U')
            && !TemplateParamNode::mangledRepresentationStartsWith('D'));
    QVERIFY(!SubstitutionNode::mangledRepresentationStartsWith('P')
            && !SubstitutionNode::mangledRepresentationStartsWith('R')
            && !SubstitutionNode::mangledRepresentationStartsWith('O')
            && !SubstitutionNode::mangledRepresentationStartsWith('C')
            && !SubstitutionNode::mangledRepresentationStartsWith('G')
            && !SubstitutionNode::mangledRepresentationStartsWith('U')
            && !SubstitutionNode::mangledRepresentationStartsWith('D'));
    QVERIFY(!CvQualifiersNode::mangledRepresentationStartsWith('P')
            && !CvQualifiersNode::mangledRepresentationStartsWith('R')
            && !CvQualifiersNode::mangledRepresentationStartsWith('O')
            && !CvQualifiersNode::mangledRepresentationStartsWith('C')
            && !CvQualifiersNode::mangledRepresentationStartsWith('G')
            && !CvQualifiersNode::mangledRepresentationStartsWith('U')
            && !CvQualifiersNode::mangledRepresentationStartsWith('D'));

    // <array-type>
    QVERIFY(!NonNegativeNumberNode::mangledRepresentationStartsWith('_'));
    QVERIFY(!ExpressionNode::mangledRepresentationStartsWith('_'));

    // <substitution>
    QVERIFY(!NonNegativeNumberNode::mangledRepresentationStartsWith('_', 36)
            && !NonNegativeNumberNode::mangledRepresentationStartsWith('t', 36)
            && !NonNegativeNumberNode::mangledRepresentationStartsWith('a', 36)
            && !NonNegativeNumberNode::mangledRepresentationStartsWith('b', 36)
            && !NonNegativeNumberNode::mangledRepresentationStartsWith('s', 36)
            && !NonNegativeNumberNode::mangledRepresentationStartsWith('i', 36)
            && !NonNegativeNumberNode::mangledRepresentationStartsWith('o', 36)
            && !NonNegativeNumberNode::mangledRepresentationStartsWith('d', 36));

    // <special-name>
    QVERIFY(!CallOffsetNode::mangledRepresentationStartsWith('V')
            && !CallOffsetNode::mangledRepresentationStartsWith('T')
            && !CallOffsetNode::mangledRepresentationStartsWith('I')
            && !CallOffsetNode::mangledRepresentationStartsWith('S')
            && !CallOffsetNode::mangledRepresentationStartsWith('c'));

    // <unscoped-name>
    QVERIFY(!UnqualifiedNameNode::mangledRepresentationStartsWith('S'));
}

void NameDemanglerAutoTest::testIncorrectlyMangledName(
    const QString &mangledName)
{
    QVERIFY(!demangler.demangle(mangledName));
}

QTEST_MAIN(NameDemanglerAutoTest)

#include "tst_namedemangler.moc"
