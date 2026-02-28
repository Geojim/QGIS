/***************************************************************************
                         qgsrastercontourlabeling.h
                         ---------------
    begin                : February 2026
    copyright            : (C) 2026 by the QGIS project
    email                : info at qgis dot org
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSRASTERCONTOURLABELING_H
#define QGSRASTERCONTOURLABELING_H

#include "qgis_core.h"
#include "qgis_sip.h"
#include "qgsrasterlabeling.h"

class QgsLineString;
class QgsNumericFormat;

#ifndef SIP_RUN

/**
 * \ingroup core
 * \brief Implements labeling for raster contour lines.
 *
 * Generates contour line geometries from a raster band using GDAL and registers
 * them with the labeling engine for placement along the contour lines.
 *
 * \note Not available in Python bindings
 *
 * \since QGIS 3.44
 */
class CORE_EXPORT QgsRasterContourLabelProvider : public QgsRasterLayerLabelProvider
{
  public:
    //! Constructs a contour label provider for the given raster  layer.
    explicit QgsRasterContourLabelProvider( QgsRasterLayer *layer );
    ~QgsRasterContourLabelProvider() override;

    void generateLabels( QgsRenderContext &context, QgsRasterPipe *pipe, QgsRasterViewPort *rasterViewPort, QgsRasterLayerRendererFeedback *feedback ) override;

    //! Sets the contour  interval for generating contour lines.
    void setContourInterval( double interval ) { mContourInterval = interval; }
    //! Sets the contour index  interval for distinguishing index contours.
    void setContourIndexInterval( double interval ) { mContourIndexInterval = interval; }
    //! Sets the input raster  band to use for contour generation.
    void setInputBand( int band ) { mInputBand = band; }
    //! Sets the  downscale factor for reducing raster resolution before contouring.
    void setDownscale( double downscale ) { mDownscale = downscale; }
    //! Sets whether to label only index contours.
    void setLabelIndexOnly( bool indexOnly ) { mLabelIndexOnly = indexOnly; }

    //! Adds a contour label along  line with the given  text.
    void addContourLabel( const QgsLineString &line, const QString &text, QgsRenderContext &context );

  private:
    double mContourInterval = 100.0;
    double mContourIndexInterval = 0.0;
    int mInputBand = 1;
    double mDownscale = 4.0;
    bool mLabelIndexOnly = false;
};

#endif

/**
 * \ingroup core
 * \brief Labeling configuration for raster contour lines.
 *
 * Produces labels placed along contour lines generated on-the-fly from raster data.
 *
 * \since QGIS 3.44
 */
class CORE_EXPORT QgsRasterLayerContourLabeling : public QgsAbstractRasterLayerLabeling
{
  public:

    QgsRasterLayerContourLabeling();
    ~QgsRasterLayerContourLabeling() override;

    QString type() const override;
    QgsRasterLayerContourLabeling *clone() const override SIP_FACTORY;
    std::unique_ptr< QgsRasterLayerLabelProvider > provider( QgsRasterLayer *layer ) const override SIP_SKIP;
    QDomElement save( QDomDocument &doc, const QgsReadWriteContext &context ) const override;
    bool accept( QgsStyleEntityVisitorInterface *visitor ) const override;
    bool requiresAdvancedEffects() const override;
    bool hasNonDefaultCompositionMode() const override;
    void multiplyOpacity( double opacityFactor ) override;
    bool isInScaleRange( double scale ) const override;

    //! Creates a QgsRasterLayerContourLabeling from a DOM element with saved configuration.
    static QgsRasterLayerContourLabeling *create( const QDomElement &element, const QgsReadWriteContext &context ) SIP_FACTORY;

    //! Returns the text format used for rendering contour labels.
    QgsTextFormat textFormat() const;

    //! Sets the text \a format used for rendering contour labels.
    void setTextFormat( const QgsTextFormat &format );

    //! Returns the numeric format used for formatting contour elevation values.
    const QgsNumericFormat *numericFormat() const;

    //! Sets the numeric \a format used for formatting contour elevation values. Ownership is transferred.
    void setNumericFormat( QgsNumericFormat *format SIP_TRANSFER );

    //! Returns whether only index contours are labeled.
    bool labelIndexOnly() const { return mLabelIndexOnly; }

    //! Sets whether only index contours should be labeled.
    void setLabelIndexOnly( bool indexOnly ) { mLabelIndexOnly = indexOnly; }

    //! Returns the label priority, where 0 is lowest and 1 is highest.
    double priority() const { return mPriority; }

    //! Sets the label \a priority, where 0 is lowest and 1 is highest.
    void setPriority( double priority ) { mPriority = priority; }

    //! Returns the label placement settings. \note Not available in Python bindings.
    const QgsLabelPlacementSettings &placementSettings() const { return mPlacementSettings; } SIP_SKIP

    //! Returns the label placement settings.
    QgsLabelPlacementSettings &placementSettings() { return mPlacementSettings; }

    //! Sets the label placement \a settings.
    void setPlacementSettings( const QgsLabelPlacementSettings &settings ) { mPlacementSettings = settings; }

    //! Returns the label thinning settings. \note Not available in Python bindings.
    const QgsLabelThinningSettings &thinningSettings() const { return mThinningSettings; } SIP_SKIP

    //! Returns the label thinning settings.
    QgsLabelThinningSettings &thinningSettings() { return mThinningSettings; }

    //! Sets the label thinning \a settings.
    void setThinningSettings( const QgsLabelThinningSettings &settings ) { mThinningSettings = settings; }

    //! Returns the z-index for label rendering order.
    double zIndex() const;

    //! Sets the z-\a index for label rendering order.
    void setZIndex( double index );

    //! Returns the maximum map scale (most zoomed in) at which labels are visible.
    double maximumScale() const;

    //! Sets the maximum map \a scale (most zoomed in) at which labels are visible.
    void setMaximumScale( double scale );

    //! Returns the minimum map scale (most zoomed out) at which labels are visible.
    double minimumScale() const;

    //! Sets the minimum map \a scale (most zoomed out) at which labels are visible.
    void setMinimumScale( double scale );

    //! Sets whether scale-based visibility is \a enabled for labels.
    void setScaleBasedVisibility( bool enabled );

    //! Returns whether scale-based visibility is enabled for labels.
    bool hasScaleBasedVisibility() const;

  private:
    bool mLabelIndexOnly = false;

    QgsTextFormat mTextFormat;
    std::unique_ptr< QgsNumericFormat > mNumericFormat;

    double mPriority = 0.5;
    QgsLabelPlacementSettings mPlacementSettings;
    QgsLabelThinningSettings mThinningSettings;
    double mZIndex = 0;

    bool mScaleVisibility = false;
    double mMaximumScale = 0;
    double mMinimumScale = 0;
};

#endif // QGSRASTERCONTOURLABELING_H
