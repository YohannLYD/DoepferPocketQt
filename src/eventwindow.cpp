#include "eventwindow.h"
#include <QHBoxLayout>

eventWindow::eventWindow(QWidget *parent) :
    QMainWindow(parent),
    _eventsTable(new QTableWidget)
{
    QWidget *mainWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(mainWidget);

    _eventsList << "Controller" << "PitchBend" << "MonoAftertouch" << "Prg-Change" << "PolyAftertouch" << "Note-On" << "Note-Off" << "" << "RPN0_MSB" << "RPN0_LSB" << "RPN1_MSB" << "RPN1_LSB" << "" << "" << "RPN127_MSB" << "RPN127_LSB" << "NRPN0_MSB" << "NRPN0_LSB" << "NRPN1_MSB XG_Multi/GS" << "NRPN1_LSB XG_Multi/GS" << "NRPN8_MSB" << "NRPN8_LSB" << "NRPN9_MSB" << "NRPN9_LSB" << "NRPN10_MSB" << "NRPN10_LSB" << "NRPN20_MSB XG Drum Instr. Cutoff" << "NRPN20_LSB XG Drum Instr. Cutoff" << "NRPN21_MSB XG Drum Instr. Resonance" << "NRPN21_LSB XG Drum Instr. Resonance" << "NRPN22_MSB XG Drum Instr. EG Attack" << "NRPN22_LSB XG Drum Instr. EG Attack" << "NRPN23_MSB XG Drum Instr. EG Decay" << "NRPN23_LSB XG Drum Instr. EG Decay" << "NRPN24_MSB XG/GS Drum Instr. Pitch Coarse" << "NRPN24_LSB XG/GS Drum Instr. Pitch Coarse" << "NRPN25_MSB XG Drum Instr. Pitch Fine" << "NRPN25_LSB XG Drum Instr. Pitch Fine" << "NRPN26_MSB XG/GS Drum Instr. Level" << "NRPN26_LSB XG/GS Drum Instr. Coarse" << "NRPN28_MSB XG/GS Drum Instr. Panorama" << "NRPN28_LSB XG/GS Drum Instr. Panorama" << "NRPN29_MSB XG/GS Drum Instr. Reverb Send" << "NRPN29_LSB XG/GS Drum Instr. Reverb Send" << "NRPN30_MSB XG/GS Drum Instr. Chorus Send" << "NRPN30_LSB XG/GS Drum Instr. Chorus Send" << "NRPN31_MSB XG/GS Drum Instr.Variation/Delay Send" << "NRPN31_LSB XG/GS Drum Instr. Variation/Delay Send" << "NRPN32_MSB" << "NRPN32_LSB" << "NRPN33_MSB" << "NRPN33_LSB" << "NRPN99_MSB" << "NRPN99_LSB" << "NRPN100_MSB" << "NRPN100_LSB" << "NRPN102_MSB" << "NRPN102_LSB" << "NRPN120_MSB EMU ABS" << "NRPN120_LSB EMU ABS" << "NRPN127_MSB SB AWE 32/64" << "NRPN127_LSB SB AWE 32/64" << "" << "" << "XG-DRUM (SetupNr=Chn) Pitch Coarse" << "XG-DRUM (SetupNr=Chn) Pitch Fine" << "XG-DRUM (SetupNr=Chn) Level" << "XG-DRUM (SetupNr=Chn) Alternate Group" << "XG-DRUM (SetupNr=Chn) Panorama" << "XG-DRUM (SetupNr=Chn) Reverb Send" << "XG-DRUM (SetupNr=Chn) Chorus Send" << "XG-DRUM (SetupNr=Chn) Variation Send" << "XG-DRUM (SetupNr=Chn) Key Assign" << "XG-DRUM (SetupNr=Chn) Rcv Note off" << "XG-DRUM (SetupNr=Chn) Rcv Note On" << "XG-DRUM (SetupNr=Chn) Cutoff Freq." << "XG-DRUM (SetupNr=Chn) Resonance" << "XG-DRUM (SetupNr=Chn) EG Attack Rate" << "XG-DRUM (SetupNr=Chn) EG Decay1 Rate";
    _eventsList << "XG-DRUM (SetupNr=Chn) EG Decay2 Rate" << "XG Multipart" << "XG AD" << "XG-Effect" << "XG-Effect 2Byte" << "XG-Reverb Type" << "XG-Chorus Type";
    _eventsList << "XG-Variation Type" << "XG-EQ" << "XG-Insertion1" << "XG-Insertion2" << "XG-Insertion 1 Type" << "XG-Insertion 2 Type" << "" << "" << "" << "" << "GS - Effect" << "MC303_Global" << "MC303_Channel" << "" << "Strings" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "" << "No-Event";


    _eventsTable->setRowCount(32);
    _eventsTable->setColumnCount(4);
    _eventsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _eventsTable->setSelectionMode(QAbstractItemView::SingleSelection);

    _eventsTable->horizontalHeader()->setHidden(true);
    _eventsTable->horizontalHeader()->setDefaultSectionSize(200);

    _eventsTable->verticalHeader()->setHidden(true);
    _eventsTable->verticalHeader()->setDefaultSectionSize(20);

    for(int r=0; r<32; r++){
        for(int c=0; c<4; c++){
            QString nom = _eventsList.at(32*c+r);
            _eventsTable->setItem(r,c,new QTableWidgetItem(nom));
            if(nom.isEmpty())
            {
                // TO DO SET GREY COLOR TO DISABLED CELLS
                // _eventsTable->item(r,c)->setBackgroundColor(QColor().black());
                _eventsTable->item(r,c)->setFlags(Qt::ItemIsEnabled);
            }
        }
    }


    mainLayout->addWidget(_eventsTable);
    mainWidget->setFixedSize(826,666);
    mainWidget->setLayout(mainLayout);
    this->setCentralWidget(mainWidget);

    connect(_eventsTable,SIGNAL(cellDoubleClicked(int,int)),parent,SLOT(updateEventCell(int,int)));
}

eventWindow::~eventWindow()
{
}

