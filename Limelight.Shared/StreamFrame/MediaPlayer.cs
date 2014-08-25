﻿namespace Limelight
{
    using Limelight_common_binding;
    using System;
    using System.Diagnostics;
    using System.Linq;
    using Windows.Graphics.Display;
    using Windows.Media.Core;
    using Windows.Media.MediaProperties;
    using Windows.UI.Xaml;
    using Windows.UI.Xaml.Controls;
    using Windows.UI.Xaml.Input;
    using Windows.UI.Xaml.Navigation;
    public sealed partial class StreamFrame : Page
    {
        #region Class Variables
        private MediaStreamSource _mss = null;
        private VideoStreamDescriptor _videoDesc = null;
        private AvStreamSource _streamSource;

        #endregion Class Variables

        #region Media Player

        /// <summary>
        /// Initialize the media element for playback
        /// </summary>
        /// <param name="streamConfig">Object containing stream configuration details</param>
        void InitializeMediaPlayer(LimelightStreamConfiguration streamConfig, AvStreamSource streamSource)
        {
            this._streamSource = streamSource;

            StreamDisplay.RealTimePlayback = true;
            StreamDisplay.AutoPlay = true;

            VideoEncodingProperties videoProperties = new VideoEncodingProperties();
            videoProperties.Subtype = MediaEncodingSubtypes.H264Es;
            videoProperties.ProfileId = H264ProfileIds.High;

            _videoDesc = new VideoStreamDescriptor(videoProperties);

            _mss = new MediaStreamSource(_videoDesc);
            _mss.BufferTime = TimeSpan.Zero;
            _mss.CanSeek = false;
            _mss.Duration = TimeSpan.Zero;
            _mss.SampleRequested += _mss_SampleRequested;

            StreamDisplay.SetMediaStreamSource(_mss);
        }

        /// <summary>
        /// Media stream source sample requested callback
        /// </summary>
        private void _mss_SampleRequested(MediaStreamSource sender, MediaStreamSourceSampleRequestedEventArgs args)
        {
            // Determine which stream needs a sample
            if (args.Request.StreamDescriptor == _videoDesc)
            {
                // Video
                _streamSource.VideoSampleRequested(args);
            }
            else
            {
                // Audio
                _streamSource.AudioSampleRequested(args);
            }
        }
        #endregion Media Player
    }
}