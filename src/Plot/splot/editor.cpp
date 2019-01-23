#include "editor.h"
#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_scale_map.h>
#include <qwt_plot_shapeitem.h>
#include <qevent.h>

class Overlay: public QwtWidgetOverlay
{
public:
    Overlay( QWidget *parent, Editor *editor ):
        QwtWidgetOverlay( parent ),
        d_editor( editor )
    {
        switch( editor->mode() )
        {
            case Editor::NoMask:
            {
                setMaskMode( QwtWidgetOverlay::NoMask );
                setRenderMode( QwtWidgetOverlay::AutoRenderMode );
                break;
            }
            case Editor::Mask:
            {
                setMaskMode( QwtWidgetOverlay::MaskHint );
                setRenderMode( QwtWidgetOverlay::AutoRenderMode );
                break;
            }
            case Editor::AlphaMask:
            {
                setMaskMode( QwtWidgetOverlay::AlphaMask );
                setRenderMode( QwtWidgetOverlay::AutoRenderMode );
                break;
            }
            case Editor::AlphaMaskRedraw:
            {
                setMaskMode( QwtWidgetOverlay::AlphaMask );
                setRenderMode( QwtWidgetOverlay::DrawOverlay );
                break;
            }
            case Editor::AlphaMaskCopyMask:
            {
                setMaskMode( QwtWidgetOverlay::AlphaMask );
                setRenderMode( QwtWidgetOverlay::CopyAlphaMask );
                break;
            }
        }
    }

protected:
    virtual void drawOverlay( QPainter *painter ) const
    {
        d_editor->drawOverlay( painter );
    }

    virtual QRegion maskHint() const
    {
        return d_editor->maskHint();
    }

private:
    Editor *d_editor;
};

Editor::Editor( QwtPlot* plot ):
    QObject( plot ),
    m_isEnabled( false ),
    m_overlay( nullptr ),
    m_mode( Mask )
{
    setEnabled( true );
    m_panner = new QwtPlotPanner(plot->canvas());
}


Editor::~Editor()
{
    delete m_overlay;
}

QwtPlot *Editor::plot()
{
    return qobject_cast<QwtPlot *>( parent() );
}

const QwtPlot *Editor::plot() const
{
    return qobject_cast<const QwtPlot *>( parent() );
}

void Editor::setMode( Mode mode )
{
    m_mode = mode;
}

Editor::Mode Editor::mode() const
{
    return m_mode;
}

void Editor::setEnabled( bool on )
{
    if ( on == m_isEnabled )
        return;

    QwtPlot *plot = qobject_cast<QwtPlot *>( parent() );
    if ( plot )
    {
        m_isEnabled = on;

        if ( on )
        {
            plot->canvas()->installEventFilter( this );
        }
        else
        {
            plot->canvas()->removeEventFilter( this );

            delete m_overlay;
            m_overlay = nullptr;
        }
    }
}

bool Editor::isEnabled() const
{
    return m_isEnabled;
}

bool Editor::eventFilter( QObject* object, QEvent* event )
{
    QwtPlot *plot = qobject_cast<QwtPlot *>( parent() );
    if ( plot && object == plot->canvas() )
    {
        switch( event->type() )
        {
            case QEvent::MouseButtonPress:
            {
                const QMouseEvent* mouseEvent =
                    dynamic_cast<QMouseEvent* >( event );

                if ( m_overlay == nullptr &&
                    mouseEvent->button() == Qt::LeftButton  )
                {
                    const bool accepted = pressed( mouseEvent->pos() );
                    if ( accepted )
                    {
                        m_overlay = new Overlay( plot->canvas(), this );
                        m_overlay->updateOverlay();
                        m_overlay->show();
                    }
                }

                break;
            }
            case QEvent::MouseMove:
            {
                if ( m_overlay )
                {
                    const QMouseEvent* mouseEvent =
                        dynamic_cast< QMouseEvent* >( event );

                    const bool accepted = moved( mouseEvent->pos() );
                    if ( accepted ){
                        m_overlay->updateOverlay();
                    }
                    event->accept();
                    return false;
                }

                break;
            }
            case QEvent::MouseButtonRelease:
            {
                const QMouseEvent* mouseEvent =
                    static_cast<QMouseEvent* >( event );

                if ( m_overlay && mouseEvent->button() == Qt::LeftButton )
                {
                    released( mouseEvent->pos() );

                    delete m_overlay;
                    m_overlay = nullptr;
                }

                break;
            }
            default:
                break;
        }

        return false;
    }

    return QObject::eventFilter( object, event );
}

bool Editor::pressed( const QPoint& pos )
{
    m_editedItem = static_cast<InteractiveShapeItem*>(itemAt( pos ));
    if ( m_editedItem )
    {
        m_panner->deleteLater();
        m_panner = nullptr;
        m_currentPos = pos;
        setItemVisible( m_editedItem, false );
        m_editedItem->pressed(pos);

        return true;
    }

    return false; // don't accept the position
}

bool Editor::moved( const QPoint& pos )
{
    if ( plot() == nullptr )
        return false;
    m_currentPos = pos;
    m_editedItem->move(pos);

    return true;
}

void Editor::released( const QPoint& pos )
{
    Q_UNUSED( pos );

    if ( m_editedItem  )
    {
        raiseItem( m_editedItem );
        setItemVisible( m_editedItem, true );
        m_panner = new QwtPlotPanner(plot()->canvas());
        m_editedItem->released(pos);
    }
}

QwtPlotShapeItem* Editor::itemAt( const QPoint& pos ) const
{
    const QwtPlot *plot = this->plot();
    if ( plot == nullptr )
        return nullptr;

    // translate pos into the plot coordinates
    double coords[ QwtPlot::axisCnt ];
    coords[ QwtPlot::xBottom ] =
        plot->canvasMap( QwtPlot::xBottom ).invTransform( pos.x() );
    coords[ QwtPlot::xTop ] =
        plot->canvasMap( QwtPlot::xTop ).invTransform( pos.x() );
    coords[ QwtPlot::yLeft ] =
        plot->canvasMap( QwtPlot::yLeft ).invTransform( pos.y() );
    coords[ QwtPlot::yRight ] =
        plot->canvasMap( QwtPlot::yRight ).invTransform( pos.y() );

    QwtPlotItemList items = plot->itemList();
    for ( int i = items.size() - 1; i >= 0; i-- )
    {
        QwtPlotItem *item = items[ i ];
        if ( item->isVisible() &&
            item->rtti() == QwtPlotItem::Rtti_PlotShape )
        {
            QwtPlotShapeItem *shapeItem = static_cast<QwtPlotShapeItem *>( item );
            const QPointF p( coords[ item->xAxis() ], coords[ item->yAxis() ] );

            if ( shapeItem->boundingRect().contains( p )
                && shapeItem->shape().contains( p ) )
            {
                return shapeItem;
            }
        }
    }

    return nullptr;
}

QRegion Editor::maskHint() const
{
    return maskHint( m_editedItem );
}

QRegion Editor::maskHint( QwtPlotShapeItem *shapeItem ) const
{
    const QwtPlot *plot = this->plot();
    if ( plot == nullptr || shapeItem == nullptr )
        return QRegion();

    const QwtScaleMap xMap = plot->canvasMap( shapeItem->xAxis() );
    const QwtScaleMap yMap = plot->canvasMap( shapeItem->yAxis() );

    QRect rect = QwtScaleMap::transform( xMap, yMap,
        shapeItem->shape().boundingRect() ).toRect();

    const int m = 5; // some margin for the pen
    return rect.adjusted( -m, -m, m, m );
}

void Editor::drawOverlay( QPainter* painter ) const
{
    const QwtPlot *plot = this->plot();
    if ( plot == nullptr || m_editedItem == nullptr )
        return;

    const QwtScaleMap xMap = plot->canvasMap( m_editedItem->xAxis() );
    const QwtScaleMap yMap = plot->canvasMap( m_editedItem->yAxis() );

    painter->setRenderHint( QPainter::Antialiasing,
        m_editedItem->testRenderHint( QwtPlotItem::RenderAntialiased ) );
    m_editedItem->draw( painter, xMap, yMap,
        plot->canvas()->contentsRect() );
}

void Editor::raiseItem( QwtPlotShapeItem *shapeItem )
{
    const QwtPlot *plot = this->plot();
    if ( plot == nullptr || shapeItem == nullptr )
        return;

    const QwtPlotItemList items = plot->itemList();
    for ( int i = items.size() - 1; i >= 0; i-- )
    {
        QwtPlotItem *item = items[ i ];
        if ( shapeItem == item )
            return;

        if ( item->isVisible() &&
            item->rtti() == QwtPlotItem::Rtti_PlotShape )
        {
            shapeItem->setZ( item->z() + 1 );
            return;
        }
    }
}

void Editor::setItemVisible( QwtPlotShapeItem *item, bool on )
{
    if ( plot() == nullptr || item == nullptr || item->isVisible() == on )
        return;

    const bool doAutoReplot = plot()->autoReplot();
    plot()->setAutoReplot( false );

    item->setVisible( on );

    plot()->setAutoReplot( doAutoReplot );

    /*
      Avoid replot with a full repaint of the canvas.
      For special combinations - f.e. using the
      raster paint engine on a remote display -
      this makes a difference.
     */

    QwtPlotCanvas *canvas =
        qobject_cast<QwtPlotCanvas *>( plot()->canvas() );
    if ( canvas )
        canvas->invalidateBackingStore();

    plot()->canvas()->update( maskHint( item ) );

}

