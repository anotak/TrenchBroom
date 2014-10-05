/*
 Copyright (C) 2010-2014 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#include "UndoableCommand.h"

#include "Exceptions.h"
#include "View/MapDocumentCommandFacade.h"

namespace TrenchBroom {
    namespace View {
        UndoableCommand::UndoableCommand(const CommandType type, const String& name) :
        Command(type, name),
        m_modificationCount(1) {}
        
        UndoableCommand::~UndoableCommand() {}

        bool UndoableCommand::performUndo(MapDocumentCommandFacade* document) {
            m_state = CommandState_Undoing;
            if (doPerformUndo(document)) {
                m_state = CommandState_Default;
                return true;
            } else {
                m_state = CommandState_Done;
                return false;
            }
        }
        
        bool UndoableCommand::isRepeatDelimiter() const {
            return doIsRepeatDelimiter();
        }
        
        bool UndoableCommand::isRepeatable(MapDocumentCommandFacade* document) const {
            return doIsRepeatable(document);
        }
        
        UndoableCommand* UndoableCommand::repeat(MapDocumentCommandFacade* document) const {
            return doRepeat(document);
        }
        
        bool UndoableCommand::collateWith(UndoableCommand* command) {
            assert(command != this);
            if (command->type() != m_type)
                return false;
            const bool didCollate = doCollateWith(command);
            if (didCollate)
                m_modificationCount += command->m_modificationCount;
            return didCollate;
        }

        void UndoableCommand::incDocumentModificationCount(MapDocumentCommandFacade* document) const {
            document->incModificationCount(m_modificationCount);
        }
        
        void UndoableCommand::decDocumentModificationCount(MapDocumentCommandFacade* document) const {
            document->decModificationCount(m_modificationCount);
        }

        bool UndoableCommand::doIsRepeatDelimiter() const {
            return false;
        }
        
        UndoableCommand* UndoableCommand::doRepeat(MapDocumentCommandFacade* document) const {
            throw CommandProcessorException("Command is not repeatable");
        }
    }
}
