#include "GridFrame.h"

GridFrame::GridFrame(wxMDIParentFrame *parent,
                     const wxString &title)
    : wxMDIChildFrame(parent,
                      GridFrameConstants::GRID_FRAME_ID,
                      title),
      m_view{this} {

    BindEvents();

}

GridFrame::~GridFrame() {
}

void GridFrame::clearGrid() {

    int clearGridDialogRetValue = m_view.showClearGridConfirmationDialog();

    if (clearGridDialogRetValue == wxID_NO) {

        return;

    }

    m_view.clearGrid();

    m_view.showGridClearedSuccessMessage();

    m_view.setPreviousStartPoint(GridFrameViewConstants::INVALID_CELL_POSITION);
    m_view.setPreviousEndPoint(GridFrameViewConstants::INVALID_CELL_POSITION);

}

void GridFrame::BindEvents() {

    wxGrid *grid = m_view.getGrid();

    grid->Bind(wxEVT_GRID_CELL_LEFT_CLICK,
               &GridFrame::OnCellSelected,
               this);

    grid->Bind(wxEVT_GRID_RANGE_SELECT,
               &GridFrame::UnselectAllCells,
               this);

    m_view.getSlider()->Bind(wxEVT_SLIDER,
                             &GridFrame::OnSliderChange,
                             this);

}

void GridFrame::OnCellSelected(wxGridEvent &event) {

    const int col = event.GetCol();
    const int row = event.GetRow();

    m_view.setCellToCurrentNode(row,
                   col);

    event.Skip();

}

void GridFrame::UnselectAllCells(wxGridRangeSelectEvent &event) {

    if (event.Selecting()) {

        m_view.getGrid()->ClearSelection();

        event.Skip();

    }

}

void GridFrame::OnSliderChange(wxCommandEvent &event) {

    int sliderValue = static_cast<wxSlider *>(event.GetEventObject())->GetValue();

    int cellSize = GridFrameViewConstants::GRID_CELL_MIN_SIZE;

    if (sliderValue != 0) {

        cellSize = cellSize
                 + static_cast<int>((sliderValue / 100.0)
                                   * cellSize);

    }

    m_view.SetCellsSize(cellSize);

}

GridFrameView &GridFrame::getView() {

    return m_view;

}
