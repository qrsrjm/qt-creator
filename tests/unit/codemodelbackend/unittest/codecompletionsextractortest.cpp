/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://www.qt.io/licensing.  For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "gtest/gtest.h"

#include "gmock/gmock-matchers.h"
#include "gmock/gmock-generated-matchers.h"

#include <clang-c/Index.h>

#include <codecompletionsextractor.h>
#include <clangcodecompleteresults.h>
#include <filecontainer.h>
#include <translationunit.h>
#include <unsavedfiles.h>

#include <QFile>

using CodeModelBackEnd::CodeCompletionsExtractor;
using CodeModelBackEnd::ClangCodeCompleteResults;
using CodeModelBackEnd::TranslationUnit;
using CodeModelBackEnd::CodeCompletion;

namespace {

using ::testing::PrintToString;

MATCHER_P4(IsCompletion, name, kind, priority, availability,
           std::string(negation ? "isn't" : "is") + " completion of name " + PrintToString(name) +
           ", kind " + PrintToString(kind) + " and priority " + PrintToString(priority)
           )
{
    while(arg.next()) {
        if (arg.currentCodeCompletion().text() == name) {
            if (arg.currentCodeCompletion().completionKind() == kind) {
                if (arg.currentCodeCompletion().priority() == quint32(priority)) {
                    if (arg.currentCodeCompletion().availability() == availability) {
                        return true;
                    } else if (!arg.peek(name)) {
                        *result_listener << "availability is " << PrintToString(arg.currentCodeCompletion().availability()) << " and not " << PrintToString(availability);
                        return false;
                    }
                } else if (!arg.peek(name)) {
                    *result_listener << "priority is " << PrintToString(arg.currentCodeCompletion().priority()) << " and not " << priority;
                    return false;
                }

            } else if (!arg.peek(name)) {
                *result_listener << "kind is " << PrintToString(arg.currentCodeCompletion().completionKind()) << " and not " << PrintToString(kind);
                return false;
            }
        }
    }

    return false;
}

ClangCodeCompleteResults getResults(const char *filePath, uint line)
{
    CodeModelBackEnd::UnsavedFiles unsavedFiles;
    TranslationUnit translationUnit(Utf8String::fromUtf8(filePath), unsavedFiles);

    return ClangCodeCompleteResults(clang_codeCompleteAt(translationUnit.cxTranslationUnit(),
                                                         translationUnit.filePath().constData(),
                                                         line,
                                                         1,
                                                         translationUnit.cxUnsavedFiles(),
                                                         translationUnit.unsavedFilesCount(),
                                                         CXCodeComplete_IncludeMacros | CXCodeComplete_IncludeCodePatterns));
}

ClangCodeCompleteResults getResultsWithUnsavedFile(const char *filePath, const char *unsavedFilePath, uint line)
{
    QFile unsavedFileContentFile(QString::fromUtf8(unsavedFilePath));
    bool isOpen = unsavedFileContentFile.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!isOpen)
         ADD_FAILURE() << "File with the unsaved content cannot be opened!";

    const Utf8String unsavedFileContent = Utf8String::fromByteArray(unsavedFileContentFile.readAll());
    const CodeModelBackEnd::FileContainer unsavedDataFileContainer(Utf8String::fromUtf8(filePath), unsavedFileContent, true);

    CodeModelBackEnd::UnsavedFiles unsavedFiles;
    unsavedFiles.update({unsavedDataFileContainer});

    TranslationUnit translationUnit(Utf8String::fromUtf8(filePath), unsavedFiles);

    return ClangCodeCompleteResults(clang_codeCompleteAt(translationUnit.cxTranslationUnit(),
                                                         translationUnit.filePath().constData(),
                                                         line,
                                                         1,
                                                         translationUnit.cxUnsavedFiles(),
                                                         translationUnit.unsavedFilesCount(),
                                                         CXCodeComplete_IncludeMacros | CXCodeComplete_IncludeCodePatterns));
}


TEST(CodeCompletionExtractor, Function)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_function.cpp", 20));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("Function"),
                                        CodeCompletion::FunctionCompletionKind,
                                        50,
                                        CodeCompletion::Available));
}

TEST(CodeCompletionExtractor, TemplateFunction)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_function.cpp", 20));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("TemplateFunction"),
                                        CodeCompletion::TemplateFunctionCompletionKind,
                                        50,
                                        CodeCompletion::Available));
}

TEST(CodeCompletionExtractor, Variable)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_variable.cpp", 4));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("Var"),
                                        CodeCompletion::VariableCompletionKind,
                                        34,
                                        CodeCompletion::Available));
}


TEST(CodeCompletionExtractor, NonTypeTemplateParameter)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_variable.cpp", 27));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("NonTypeTemplateParameter"),
                                        CodeCompletion::VariableCompletionKind,
                                        34,
                                        CodeCompletion::Available));
}


TEST(CodeCompletionExtractor, VariableReference)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_variable.cpp", 12));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("Var"),
                                        CodeCompletion::VariableCompletionKind,
                                        34,
                                        CodeCompletion::Available));
}

TEST(CodeCompletionExtractor, Parameter)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_variable.cpp", 4));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("Parameter"),
                                        CodeCompletion::VariableCompletionKind,
                                        34,
                                        CodeCompletion::Available));
}

TEST(CodeCompletionExtractor, Field)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_variable.cpp", 20));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("Field"),
                                        CodeCompletion::VariableCompletionKind,
                                        35,
                                        CodeCompletion::Available));
}

TEST(CodeCompletionExtractor, Class)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_class.cpp", 20));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("Class"),
                                        CodeCompletion::ClassCompletionKind,
                                        50,
                                        CodeCompletion::Available));
}

TEST(CodeCompletionExtractor, Struct)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_class.cpp", 20));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("Struct"),
                                        CodeCompletion::ClassCompletionKind,
                                        50,
                                        CodeCompletion::Available));
}

TEST(CodeCompletionExtractor, Union)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_class.cpp", 20));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("Union"),
                                        CodeCompletion::ClassCompletionKind,
                                        50,
                                        CodeCompletion::Available));
}

TEST(CodeCompletionExtractor, TemplateTypeParameter)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_class.cpp", 20));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("TemplateTypeParameter"),
                                        CodeCompletion::ClassCompletionKind,
                                        34,
                                        CodeCompletion::Available));
}

TEST(CodeCompletionExtractor, TemplateClass)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_class.cpp", 20));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("TemplateClass"),
                                        CodeCompletion::TemplateClassCompletionKind,
                                        50,
                                        CodeCompletion::Available));
}

TEST(CodeCompletionExtractor, TemplateTemplateParameter)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_class.cpp", 20));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("TemplateTemplateParameter"),
                                        CodeCompletion::TemplateClassCompletionKind,
                                        34,
                                        CodeCompletion::Available));
}

TEST(CodeCompletionExtractor, ClassTemplatePartialSpecialization)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_class.cpp", 20));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("ClassTemplatePartialSpecialization"),
                                        CodeCompletion::TemplateClassCompletionKind,
                                        50,
                                        CodeCompletion::Available));
}

TEST(CodeCompletionExtractor, Namespace)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_namespace.cpp", 20));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("Namespace"),
                                        CodeCompletion::NamespaceCompletionKind,
                                        75,
                                        CodeCompletion::Available));
}

TEST(CodeCompletionExtractor, NamespaceAlias)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_namespace.cpp", 20));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("NamespaceAlias"),
                                        CodeCompletion::NamespaceCompletionKind,
                                        75,
                                        CodeCompletion::Available));
}

TEST(CodeCompletionExtractor, Enumeration)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_enumeration.cpp", 20));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("Enumeration"),
                                        CodeCompletion::EnumerationCompletionKind,
                                        50,
                                        CodeCompletion::Available));
}

TEST(CodeCompletionExtractor, Enumerator)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_enumeration.cpp", 20));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("Enumerator"),
                                        CodeCompletion::EnumeratorCompletionKind,
                                        65,
                                        CodeCompletion::Available));
}

TEST(CodeCompletionExtractor, Constructor)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_constructor.cpp", 20));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("Constructor"),
                                        CodeCompletion::ConstructorCompletionKind,
                                        50,
                                        CodeCompletion::Available));
}

TEST(CodeCompletionExtractor, Destructor)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_constructor.cpp", 20));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("~Constructor"),
                                        CodeCompletion::DestructorCompletionKind,
                                        34,
                                        CodeCompletion::Available));
}

TEST(CodeCompletionExtractor, Method)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_function.cpp", 20));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("Method"),
                                        CodeCompletion::FunctionCompletionKind,
                                        34,
                                        CodeCompletion::Available));
    ASSERT_FALSE(extractor.currentCodeCompletion().hasParameters());
}

TEST(CodeCompletionExtractor, MethodWithParameters)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_function.cpp", 20));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("MethodWithParameters"),
                                        CodeCompletion::FunctionCompletionKind,
                                        34,
                                        CodeCompletion::Available));
    ASSERT_TRUE(extractor.currentCodeCompletion().hasParameters());
}

TEST(CodeCompletionExtractor, Slot)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_function.cpp", 20));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("Slot"),
                                        CodeCompletion::SlotCompletionKind,
                                        34,
                                        CodeCompletion::Available));
}

TEST(CodeCompletionExtractor, Signal)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_function.cpp", 20));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("Signal"),
                                        CodeCompletion::SignalCompletionKind,
                                        34,
                                        CodeCompletion::Available));
}

TEST(CodeCompletionExtractor, MacroDefinition)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_variable.cpp", 35));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("MacroDefinition"),
                                        CodeCompletion::PreProcessorCompletionKind,
                                        70,
                                        CodeCompletion::Available));
}

TEST(CodeCompletionExtractor, FunctionMacro)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_function.cpp", 35));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("FunctionMacro"),
                                        CodeCompletion::FunctionCompletionKind,
                                        70,
                                        CodeCompletion::Available));
}

TEST(CodeCompletionExtractor, IntKeyword)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_function.cpp", 20));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("int"),
                                        CodeCompletion::KeywordCompletionKind,
                                        50,
                                        CodeCompletion::Available));
}

TEST(CodeCompletionExtractor, SwitchKeyword)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_function.cpp", 20));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("switch"),
                                        CodeCompletion::KeywordCompletionKind,
                                        40,
                                        CodeCompletion::Available));
}

TEST(CodeCompletionExtractor, ClassKeyword)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_function.cpp", 20));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("class"),
                                        CodeCompletion::KeywordCompletionKind,
                                        50,
                                        CodeCompletion::Available));
}

TEST(CodeCompletionExtractor, DeprecatedFunction)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_function.cpp", 20));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("DeprecatedFunction"),
                                        CodeCompletion::FunctionCompletionKind,
                                        34,
                                        CodeCompletion::Deprecated));
}

TEST(CodeCompletionExtractor, NotAccessibleFunction)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_function.cpp", 20));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("NotAccessibleFunction"),
                                        CodeCompletion::FunctionCompletionKind,
                                        36,
                                        CodeCompletion::NotAccessible));
}

TEST(CodeCompletionExtractor, NotAvailableFunction)
{
    ClangCodeCompleteResults completeResults(getResults("data/complete_extractor_function.cpp", 20));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("NotAvailableFunction"),
                                        CodeCompletion::FunctionCompletionKind,
                                        34,
                                        CodeCompletion::NotAvailable));
}

TEST(CodeCompletionExtractor, UnsavedFile)
{
    ClangCodeCompleteResults completeResults(getResultsWithUnsavedFile("data/complete_extractor_function.cpp",
                                                                       "data/complete_extractor_function_unsaved.cpp",
                                                                       20));

    CodeCompletionsExtractor extractor(completeResults.data());

    ASSERT_THAT(extractor, IsCompletion(Utf8StringLiteral("Method2"),
                                        CodeCompletion::FunctionCompletionKind,
                                        34,
                                        CodeCompletion::Available));
}
}