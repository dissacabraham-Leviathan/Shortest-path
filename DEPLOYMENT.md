# Deployment Guide - CIT Campus Navigator

## Quick Deploy (Simplest Method)

### Option 1: Open Locally
1. Double-click `index.html` in your file browser
2. Your default browser will open the landing page
3. Click "Launch Navigator" to start using the map

### Option 2: Local Web Server
```bash
# Using Python 3
python -m http.server 8000

# Using Python 2
python -m SimpleHTTPServer 8000

# Using Node.js (if you have http-server installed)
npx http-server -p 8000

# Using PHP
php -S localhost:8000
```

Then open: `http://localhost:8000`

## Production Deployment

### Static Hosting (Recommended)

#### GitHub Pages
```bash
# 1. Create a new repository on GitHub
# 2. Push your files
git init
git add .
git commit -m "Initial commit - CIT Campus Navigator"
git branch -M main
git remote add origin https://github.com/yourusername/cit-campus-navigator.git
git push -u origin main

# 3. Enable GitHub Pages
# Go to Settings > Pages > Source > main branch > Save
# Your site will be live at: https://yourusername.github.io/cit-campus-navigator/
```

#### Netlify
```bash
# 1. Install Netlify CLI
npm install -g netlify-cli

# 2. Deploy
netlify deploy --prod

# Follow the prompts to create a new site
# Your site will be live at: https://your-site-name.netlify.app
```

#### Vercel
```bash
# 1. Install Vercel CLI
npm install -g vercel

# 2. Deploy
vercel --prod

# Your site will be live at: https://your-project.vercel.app
```

#### Cloudflare Pages
1. Go to https://pages.cloudflare.com
2. Connect your Git repository
3. Deploy automatically on every push

### Traditional Web Hosting

#### Upload via FTP/SFTP
```
1. Connect to your web host via FTP client (FileZilla, etc.)
2. Upload all files to public_html or www directory
3. Access via your domain: https://yourdomain.com
```

#### cPanel File Manager
```
1. Log into cPanel
2. Open File Manager
3. Navigate to public_html
4. Upload all files
5. Extract if uploaded as ZIP
```

## File Structure

```
cit-campus-navigator/
├── index.html              # Landing page (entry point)
├── map.html                # Main interactive map
├── QUICKSTART.html         # User guide
├── README.md               # Documentation
├── FEATURES.md             # Feature comparison
├── DEPLOYMENT.md           # This file
├── collegemap.png          # Reference image
├── shortest.c              # C implementation (reference)
├── azure-maps-animations.js        # Animation library (optional)
├── azure-maps-animations.min.js    # Minified version
├── Animate a GPS trace.html        # Example (optional)
├── Animate along a route path.html # Example (optional)
└── build.js                # Build script (optional)
```

## Required Files (Minimum)

For basic deployment, you only need:
- `index.html`
- `map.html`
- `QUICKSTART.html`

Optional files:
- `collegemap.png` (reference only)
- `shortest.c` (reference only)
- `azure-maps-animations.js` (not used in current implementation)
- Animation examples (for reference)

## Configuration

### Update Site Title/Branding
Edit `index.html` and `map.html`:
```html
<title>Your Campus Name - Navigator</title>
```

### Customize Colors
Edit CSS in `map.html`:
```css
:root {
  --accent:#4285f4;      /* Primary blue */
  --green-pin:#34a853;   /* Destination pin */
  --red-pin:#e44;        /* Start pin */
}
```

### Add Google Analytics (Optional)
Add before `</head>` in all HTML files:
```html
<!-- Google Analytics -->
<script async src="https://www.googletagmanager.com/gtag/js?id=GA_MEASUREMENT_ID"></script>
<script>
  window.dataLayer = window.dataLayer || [];
  function gtag(){dataLayer.push(arguments);}
  gtag('js', new Date());
  gtag('config', 'GA_MEASUREMENT_ID');
</script>
```

## Performance Optimization

### Enable Compression
Add `.htaccess` file (Apache):
```apache
# Enable GZIP compression
<IfModule mod_deflate.c>
  AddOutputFilterByType DEFLATE text/html text/css text/javascript application/javascript image/svg+xml
</IfModule>

# Enable browser caching
<IfModule mod_expires.c>
  ExpiresActive On
  ExpiresByType text/html "access plus 1 hour"
  ExpiresByType text/css "access plus 1 month"
  ExpiresByType application/javascript "access plus 1 month"
  ExpiresByType image/svg+xml "access plus 1 month"
</IfModule>
```

### CDN Integration (Optional)
For faster global access, use a CDN:
- Cloudflare (free tier available)
- AWS CloudFront
- Google Cloud CDN

## Security Headers

Add to `.htaccess` or server config:
```apache
# Security headers
Header set X-Content-Type-Options "nosniff"
Header set X-Frame-Options "SAMEORIGIN"
Header set X-XSS-Protection "1; mode=block"
Header set Referrer-Policy "strict-origin-when-cross-origin"
```

## Mobile Optimization

Already included:
- ✅ Responsive viewport meta tag
- ✅ Touch gesture support
- ✅ Mobile-friendly controls
- ✅ Optimized for small screens

## Browser Compatibility

Tested and working on:
- Chrome 90+ ✅
- Firefox 88+ ✅
- Safari 14+ ✅
- Edge 90+ ✅
- Mobile browsers ✅

## Troubleshooting

### Map not displaying
- Check browser console for errors (F12)
- Ensure all files are uploaded
- Verify file paths are correct

### Animations not working
- Check if browser supports SVG animations
- Try a different browser
- Clear browser cache

### Touch gestures not working
- Ensure viewport meta tag is present
- Test on actual device (not just browser emulation)
- Check for JavaScript errors

### Slow performance
- Enable GZIP compression
- Use a CDN
- Optimize images (if added)
- Enable browser caching

## Custom Domain Setup

### Cloudflare (Free SSL)
1. Add your domain to Cloudflare
2. Update nameservers at your registrar
3. Enable "Always Use HTTPS"
4. Enable "Auto Minify" for HTML/CSS/JS

### Let's Encrypt SSL (Free)
```bash
# Using Certbot
sudo certbot --apache -d yourdomain.com -d www.yourdomain.com
```

## Monitoring & Analytics

### Recommended Tools
- Google Analytics (free)
- Google Search Console (free)
- Cloudflare Analytics (free)
- Uptime monitoring (UptimeRobot - free)

## Backup Strategy

### Automated Backups
```bash
# Daily backup script
#!/bin/bash
DATE=$(date +%Y%m%d)
tar -czf backup-$DATE.tar.gz *.html *.md *.png
# Upload to cloud storage
```

### Version Control
```bash
# Use Git for version control
git add .
git commit -m "Update: description of changes"
git push origin main
```

## Support & Maintenance

### Regular Updates
- Test on new browser versions
- Update documentation as needed
- Add new features based on user feedback
- Monitor performance metrics

### User Feedback
- Add feedback form (optional)
- Monitor analytics for usage patterns
- Track error logs
- Collect feature requests

## Scaling Considerations

### High Traffic
- Use CDN for static assets
- Enable caching headers
- Consider serverless hosting
- Monitor bandwidth usage

### Future Enhancements
- Add backend API for dynamic data
- Implement user accounts
- Add real-time features
- Integrate with campus systems

## License & Credits

Add `LICENSE` file if open-sourcing:
```
MIT License

Copyright (c) 2024 Coimbatore Institute of Technology

Permission is hereby granted, free of charge, to any person obtaining a copy...
```

## Contact & Support

For deployment issues:
1. Check browser console (F12)
2. Review this deployment guide
3. Check GitHub issues (if applicable)
4. Contact IT support

---

**Deployment Checklist:**
- [ ] All files uploaded
- [ ] index.html accessible
- [ ] map.html working correctly
- [ ] Animations functioning
- [ ] Mobile responsive
- [ ] SSL certificate installed (production)
- [ ] Analytics configured (optional)
- [ ] Backup system in place
- [ ] Documentation updated
- [ ] Testing completed

**Your site is ready to go! 🚀**
